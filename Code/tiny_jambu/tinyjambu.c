#include <stdio.h>
#include <string.h>
#include "encrypt.c"

void encrypt(const unsigned char* m, unsigned long long mlen, const unsigned char* k);
void decrypt(const unsigned char* c, unsigned long long clen, const unsigned char* k);

int main(void){
    // variables
    const unsigned char m[] = {0x00};                                                   // plaintext
    unsigned long long mlen = sizeof(m);                                                // plaintext length
    const unsigned char c[] = {0xA1, 0x75, 0xD5, 0xB5, 0xC1, 0xEE, 0x4A, 0x0F, 0xA1};   // ciphertext
    unsigned long long clen = sizeof(c);                                                // ciphertext length pointer
    const unsigned char k[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F}; // 128-bit key

    encrypt(m, mlen, k);
    decrypt(c, clen, k);
    return 0;
}
void encrypt(const unsigned char* m, unsigned long long mlen, const unsigned char* k){
    unsigned long long c_length = 80;                               // ciphertext length
    unsigned char c[c_length];                                      // ciphertext
    unsigned long long *clen = &c_length;                           // ciphertext length pointer
    const unsigned char ad[] = {0x00};                              // associated data
    unsigned long long adlen = sizeof(ad);                          // associated data length
    const unsigned char *nsec;                                      // secret message number
    const unsigned char npub[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};  // public message number

    crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);

    printf("\n");
    printf("Ciphertext = ");
    for (int i = 0; i < c_length; i++){
        printf("%02X", c[i]);
    }
    printf("\n");
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