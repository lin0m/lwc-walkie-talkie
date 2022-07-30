#include<stdio.h>
#include "src/ed25519.h"
#include<string.h>

/**
 * This file is for Bob in the X3DH protocol
 * This file does the following:
 * 1. Generate a long-term identity key pair ID_b
 * 2. Generate a Signed Prekey for Bob 
 * 3. Generate Bob's Signed prekey signature Sig(IK_b, Encode(SPK_b))
 * 4. Send to Server the public keys
 * 5. Wait for Alice to verify and receives IK_a, EK_a, ciphertext encrypted with SK
 * 6. Verify signature and calculate SK 
 * 7. Decrypt the initialcipher text with SK 
 * https://github.com/massar/rfc6234/blob/master/hkdf.c
 * https://www.reddit.com/r/cryptography/comments/q8r2ld/x3dh_what_does_it_provide_and_why_is_it_more/
 * https://github.com/PaulLaux/X3DH-Key-Exchange/blob/master/x3dh.cpp
 */

void Encode(const unsigned char *value) {
    //As per the signal protocol 
    //https://stackoverflow.com/questions/73159050/create-an-encoding-function-for-a-32-byte-array-in-c-and-decode-too-from-a-pyth
    //Convert the python encoding fn to C (not sure how to do it)
};

void SendtoServer(unsigned char *id_public_key, unsigned char *spk_public_key, unsigned char *spk_signature){};

void ReceiveFromAlice(unsigned char *alice_identity_public_key, unsigned char *alice_ephemeral_public_key, unsigned char *initial_ciphertext){};

int main() {
     unsigned char id_public_key[32], id_private_key[64], seed[32], scalar[32];
     unsigned char spk_public_key[32], spk_private_key[64];
     unsigned char spk_signature[64];
     unsigned char alice_identity_public_key[32], alice_ephemeral_public_key[32];
     unsigned char initial_ciphertext[64];
     unsigned char shared_secret[32]; //Secret Key generated by Bob
     unsigned char ad[32];
     //DH outputs
     unsigned char dh1[32], dh2[32], dh3[32];
    
    //Generating long-term Identity Key pair for Bob
    ed25519_create_seed(seed);
    ed25519_create_keypair(id_public_key, id_private_key, seed);

    //Generate SignedPreKey Pair for bob
    ed25519_create_seed(seed);
    ed25519_create_keypair(spk_public_key, spk_private_key, seed);

    //TODO: Will need to remove the message and message_len from this function. As of now keeping it. 
    const unsigned char message[] = "Hey Alice, it's me Bob!";
    const int message_len = strlen((char*) message);
    ed25519_sign(spk_signature, message, message_len, id_public_key, id_private_key);

    //Send to Server:
    SendtoServer(id_public_key,spk_public_key,spk_signature);

    /**
     * WAIT TO RECEIVE KEYS FROM ALICE
     */

    //IF KEYS being sent from Alice
    //Calculating DH outputs
    //DH1 = DH(IKA, SPKB)
    ed25519_key_exchange(dh1, alice_identity_public_key, spk_private_key);

    //DH2 = DH(EKA, IKB)
    ed25519_key_exchange(dh2, alice_ephemeral_public_key, id_private_key);

    //DH3 = DH(EKA, SPKB)
    ed25519_key_exchange(dh2, alice_ephemeral_public_key, spk_private_key);

    //TODO: Maybe Construct AD 
    //TODO: HKDF
    // https://stackoverflow.com/questions/19147619/what-implementions-of-ed25519-exist - as per this link this code needs to hash the keys before sending


    return 0;
}