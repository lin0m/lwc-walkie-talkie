#include <stdio.h>
#include <string.h>
#ifdef USE_PICO
    #include "pico/stdlib.h"
    #include "hardware/pio.h"
#endif
#include "crypto_aead.h"

int main(void){
    #ifdef USE_PICO
        stdio_init_all();
    #endif
    /*
    Count = 35
    Key = 000102030405060708090A0B0C0D0E0F
    Nonce = 000102030405060708090A0B
    PT = 00
    AD = 00
    CT = A175D5B5C1EE4A0FA1
    */

    // variables
    unsigned long long c_length = 80;
    unsigned char c[c_length];           // ciphertext
    unsigned long long *clen = &c_length;   // ciphertext length
    const unsigned char m[] = {0x00};     // plaintext
    unsigned long long mlen = sizeof(m);    // plaintext length
    const unsigned char ad[] = {0x00};    // associated data
    unsigned long long adlen = sizeof(ad);   // associated data length
    const unsigned char *nsec;  // nonce
    const unsigned char npub[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B} ;  // nonce
    const unsigned char k[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};     // 128-bit key

    crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);
    
    printf("Key = ");
    for (unsigned long long i = 0; i < sizeof(k); i++) {
        printf("%02X", k[i]);
    }
    printf("\n");
    printf("Nonce = ");
    for (unsigned long long i = 0; i < sizeof(npub); i++) {
        printf("%02X", npub[i]);
    }
    printf("\n");
    printf("Plaintext = ");
    for (unsigned long long i = 0; i < sizeof(m); i++) {
        printf("%02X", m[i]);
    }
    printf("\n");
    printf("Associated Data = ");
    for (unsigned long long i = 0; i < sizeof(ad); i++) {
        printf("%02X", ad[i]);
    }
    printf("\n");
    printf("Ciphertext = ");
    for (unsigned long long i = 0; i < c_length; i++) {
        printf("%02X", c[i]);
    }
    printf("\n");
    return 0;
}