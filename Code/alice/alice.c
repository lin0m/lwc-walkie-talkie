#include <stdio.h>
#include <string.h>
#include "encrypt.c"
#include "../X3DH/ed25519/src/ed25519.h"
#include "../X3DH/rfc6234/sha.h"

/**
 * This code is for Alice (Bob in the case of X3DH)
 * Alice does the following: 
 * 1. Connect to the server
 * 2. Send Prekey bundle 
 * 3. Listen to Server and Wait for the Server to send the encrypted audio 
 * 4. Decrypt the audio
 * 5. Play the audio
 */

void decrypt(const unsigned char* c, unsigned long long clen, const unsigned char* k);
void ConnectToServer(){};
void SendtoServer(const unsigned char *id_public_key, unsigned char *spk_public_key, const unsigned char *spk_signature, int message_type){};
int ReceiveFromAlice(unsigned char *alice_identity_public_key, unsigned char *alice_ephemeral_public_key, unsigned char *initial_ciphertext, int message_type){};
void get_dh_output(unsigned char* alice_identity_public_key, unsigned char* spk_private_key, unsigned char* id_private_key, 
                    unsigned char* alice_ephemeral_public_key, unsigned char* dh_final);
void get_shared_key(unsigned char *dh_final, SHAversion whichSha, const unsigned char *salt_len, const unsigned char *info,
                    unsigned char* output_key, int okm_len);

int main(void){
    /*--------------------------------DEFINING VARIABLES FOR X3DH--------------------------*/
    unsigned char id_public_key[32]; //Bob's Identity Public Key
    unsigned char id_private_key[64]; //Bob's Identity Private Key
    unsigned char seed[32]; //Seed to generate new keys
    unsigned char scalar[32]; //Scalar to add to modify key
    unsigned char spk_public_key[32]; //Bob's Signed prekey public
    unsigned char spk_private_key[64]; //Bob's Signed prekey private
    unsigned char spk_signature[64]; //Bob's Signed prekey signature
    unsigned char alice_identity_public_key[32]; //Alice public identity key
    unsigned char alice_ephemeral_public_key[32]; //Alice ephemeral/generated key
    unsigned char dh_final[96]; //Store the concatenation of the DH outputs 
    unsigned char hex_hkdf_output[128]; //Final key to be passed to TinyJambu
    unsigned char ciphertext[32]; //As of now we have kept it as 32 bits -- will need to verify
    //TO Compile: gcc alice.c src/*.c ../rfc6234/*.c -o alice.exe

    /*----------------------------------CONNECT TO SERVER AND SEND PREKEY BUNDLE----------------------------------*/

    //CONNECTING TO SERVER
    ConnectToServer();

     //Generating long-term Identity Key pair for Bob
    ed25519_create_seed(seed); //create random seed
    ed25519_create_keypair(id_public_key, id_private_key, seed); //create keypair out of seed 

    //Generate SignedPreKey Pair for bob
    ed25519_create_seed(seed); //create random seed 
    ed25519_create_keypair(spk_public_key, spk_private_key, seed); //create keypair out of seed
 
    ed25519_sign(spk_signature, id_public_key, id_private_key);

    //Send keys to server - with my message type to indicate to the server that this is the prekey bundle 
    SendtoServer(id_public_key, spk_public_key, spk_signature, 1);
    

    /*-------------------------------------START X3DH--------------------------------------*/
    // loop until message received

    while(ReceiveFromAlice(alice_identity_public_key, alice_ephemeral_public_key, ciphertext, 3) == 1)
    {
        /**
         * Waiting for server to send response of the encrypted audio
         */
        continue;
    }

    
    get_dh_output(alice_identity_public_key, spk_private_key, id_private_key,alice_ephemeral_public_key, dh_final);
    get_shared_key(dh_final, SHA512, NULL, NULL, hex_hkdf_output, 128);


    /*------LOOP "LISTEN TO SERVER" &"DECRYPT AUDIO" & "PLAY DECRYPTED AUDIO" FOREVER-----*/
    /*-----------------------------------LISTEN TO SERVER----------------------------------*/
    // loop until message received

    /*-----------------------------------DECRYPT AUDIO------------------------------------*/
    // variables
    const unsigned char c[] = {0xA1, 0x75, 0xD5, 0xB5, 0xC1, 0xEE, 0x4A, 0x0F, 0xA1};   // ciphertext
    unsigned long long clen = sizeof(c);                                                // ciphertext length pointer
    const unsigned char k[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F}; // 128-bit key
    decrypt(c, clen, hex_hkdf_output);

    /*--------------------------------PLAY DECRYPTED AUDIO--------------------------------*/

    return 0;
}

void decrypt(const unsigned char* c, unsigned long long clen, const unsigned char* k){
    unsigned long long m_length = 80;                               // plaintext length
    unsigned char m[m_length];                                      // plaintext
    unsigned long long *mlen = &m_length;                           // plaintext length pointer
    const unsigned char ad[] = {0x00};                              // associated data
    unsigned long long adlen = sizeof(ad);                          // associated data length
    unsigned char *nsec;                                            // secret message number
    const unsigned char npub[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};  // public message number

    crypto_aead_decrypt(m, mlen, nsec, c, clen, ad, adlen, npub, k);

    // print
    printf("Plaintext = ");
    for (int i = 0; i < m_length; i++){
        printf("%02X", m[i]);
    }
    printf("\n");
}


void get_dh_output(unsigned char* alice_identity_public_key, unsigned char* spk_private_key, unsigned char* id_private_key, 
                    unsigned char* alice_ephemeral_public_key, unsigned char* dh_final)
{
    //DH outputs
    unsigned char dh1[32], dh2[32], dh3[32]; //DH exchanges - no opk so only 3 outputs

    //DH1 = DH(IKA, SPKB)
    ed25519_key_exchange(dh1, alice_identity_public_key, spk_private_key);

    //DH2 = DH(EKA, IKB)
    ed25519_key_exchange(dh2, alice_ephemeral_public_key, id_private_key);

    //DH3 = DH(EKA, SPKB)
    ed25519_key_exchange(dh3, alice_ephemeral_public_key, spk_private_key);

    //Concatenating dh outputs
    strcat(dh_final, dh1);
    strcat(dh_final, dh2);
    strcat(dh_final, dh3);
}


void get_shared_key(unsigned char *dh_final, SHAversion whichSha, const unsigned char *salt, const unsigned char *info,
     unsigned char* output_key, int okm_len){
    int salt_len; //The length of the salt value (a non-secret random value) (ignored if SALT==NULL)
    int info_len; // The length of optional context and application (ignored if info==NULL)
    int ikm_len; //The length of the input keying material
    uint8_t okm_integer[okm_len];
    ikm_len = 96;
    printf("%d\n", ikm_len);
    if(salt == NULL) salt_len = 0;
    if(info == NULL) info_len = 0;



    if(hkdf(whichSha,salt,salt_len,dh_final,ikm_len,info,info_len,okm_integer,okm_len) == 0)
    {
        printf("HKDF is valid\n");
    } else {
        fprintf(stderr, "HKDF is invalid\n");
    }

    for(int i=0; i<okm_len;i++)
    {
        output_key[i] = okm_integer[i];
        printf("%d\n", output_key[i]);
    }

}