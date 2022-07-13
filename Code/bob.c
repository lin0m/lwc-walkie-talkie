// This is a mess so we need to fix it but "it works"
#include <stdio.h>
#include <string.h>
#include "encrypt.c"

int main(void){

    /* Example to try:
    Count = 35
    Key = 000102030405060708090A0B0C0D0E0F
    Nonce = 000102030405060708090A0B
    PT = 00
    AD = 00
    CT = A175D5B5C1EE4A0FA1
    */

    // // encryption variables
    // unsigned long long c_length = 9;                                // ciphertext length
    // unsigned char c[c_length];                                      // ciphertext
    // unsigned long long *clen = &c_length;                           // ciphertext length pointer
    // const unsigned char m[] = {0x00};                               // plaintext
    // unsigned long long mlen = sizeof(m);                            // plaintext length
    // const unsigned char ad[] = {0x00};                              // associated data
    // unsigned long long adlen = sizeof(ad);                          // associated data length
    // const unsigned char *nsec;                                      // secret message number
    // const unsigned char npub[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};                      // public message number
    // const unsigned char k[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F}; // 128-bit key

    // decryption variables
    const unsigned char c[] = {0xA1, 0x75, 0xD5, 0xB5, 0xC1, 0xEE, 0x4A, 0x0F, 0xA1}; // ciphertext
    unsigned long long clen = sizeof(c);                            // ciphertext length pointer
    unsigned long long m_length = 1;                                // plaintext length
    unsigned char m[m_length];                                      // plaintext
    unsigned long long *mlen = &m_length;                           // plaintext length pointer
    const unsigned char ad[] = {0x00};                              // associated data
    unsigned long long adlen = sizeof(ad);                          // associated data length
    unsigned char *nsec;                                            // secret message number
    const unsigned char npub[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};                      // public message number
    const unsigned char k[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F}; // 128-bit key

    // // encrypt
    // crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);
    
    // // print
    // printf("Key = ");
    // for (int i = 0; i < sizeof(k); i++){
    //     printf("%02X", k[i]);
    // }
    // printf("\n");
    // printf("Ciphertext = ");
    // for (int i = 0; i < sizeof(c); i++){
    //     printf("%02X", c[i]);
    // }
    // printf("\n");

    // decrypt
    crypto_aead_decrypt(m, mlen, nsec, c, clen, ad, adlen, npub, k);

    // print
    printf("Plaintext = ");
    for (int i = 0; i < sizeof(m); i++){
        printf("%02X", m[i]);
    }
    printf("\n");
    return 0;
}
