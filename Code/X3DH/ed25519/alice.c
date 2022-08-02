#include<stdio.h>
#include "src/ed25519.h"
#include<string.h>


// /**
//  * This file is for Alice in the X3DH protocol
//  * This file does the following:
//  * 1. Create long-term identity key pair for Alice
//  * 1. Fetch Prekey Bundle
//  * 2. Generate a long-term identity key pair ID_a
//  * 3. Verify signature of Bob's 
//  * 4. Generate Ephemeral Key Pair EK_a
//  * 5. Calculate SK 
//  * 6. Encrypt message with SK 
//  */

// unsigned char FetchPreKeyBundle(unsigned char *bob_id_public_key, unsigned char *bob_spk_public_key, unsigned char *bob_spk_signature){};
// void Encode(const unsigned char *value){};

// int main() {
//     unsigned char bob_id_public_key[32];            // identity key
//     unsigned char bob_spk_public_key[32];           // signed prekey
//     unsigned char bob_spk_signature[64];            // prekey signature
//     unsigned char id_public_key[32], id_private_key[64], seed[32], scalar[32];
//     unsigned char ephemeral_public_key[32];
//     unsigned char ephemeral_private_key[64]; 
//     const unsigned char message[] = "Hey Bob, this is Alice!";
//     const int message_len = strlen((char*) message);
//     unsigned char dh1[32], dh2[32], dh3[32];

//     //Creating Long-term keypair for Alice 
//     ed25519_create_seed(seed);
//     ed25519_create_keypair(id_public_key, id_private_key, seed);

//     FetchPreKeyBundle(bob_id_public_key, bob_spk_public_key, bob_spk_signature);
//     if (ed25519_verify(bob_spk_signature, bob_id_public_key)) {
//         printf("valid signature\n");
//     } else {
//         printf("invalid signature\n");
//         // Abort();
//     }

//     //Generate Ephemeral Key pair
//     ed25519_create_seed(seed);
//     ed25519_create_keypair(ephemeral_public_key, ephemeral_private_key, seed);

//     //Generate SK with DH
//     // DH1 = DH(IKA, SPKB)
//     ed25519_key_exchange(dh1, bob_id_public_key, id_private_key);

//     // DH2 = DH(EKA, IKB)
//     ed25519_key_exchange(dh1, bob_id_public_key, ephemeral_private_key);

//     // DH3 = DH(EKA, SPKB)
//     ed25519_key_exchange(dh2, bob_spk_public_key, ephemeral_private_key);

//     //TODO: Maybe Construct AD as per the signal protocol (i.e. safety number)
//     //TODO: HKDF
//     //TODO: Encrypt with some aead encryption scheme (not sure which ones) 
//     // https://stackoverflow.com/questions/19147619/what-implementions-of-ed25519-exist - as per this link this code needs to hash the keys before sending
//     printf("\nEnd of File");
//     return 0;
// }

unsigned char* returnFunction();
int main(){
    unsigned char test[32] = "hello";
    printf("Test: %s \n", test);
    test = returnFunction();
    printf("Test: %s \n", test);
    return 0;
}

unsigned char* returnFunction(){
    unsigned char *msg = malloc(32);
    msg = "Hi hi hi";
    return msg;
}