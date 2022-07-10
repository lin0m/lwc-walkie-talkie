#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "crypto_aead.h"

int main(void){
    stdio_init_all();
    /*
    Count = 35
    Key = 000102030405060708090A0B0C0D0E0F
    Nonce = 000102030405060708090A0B
    PT = 00
    AD = 00
    CT = A175D5B5C1EE4A0FA1
    */

    // variables
    unsigned char c[80];           // ciphertext
    unsigned long long *clen;   // ciphertext length
    const unsigned char m[] = "00";     // plaintext
    unsigned long long mlen = sizeof(m);    // plaintext length
    const unsigned char ad[] = "00";    // associated data
    unsigned long long adlen = sizeof(ad);   // associated data length
    const unsigned char *nsec;  // nonce
    const unsigned char npub[] = "000102030405060708090A0B";  // nonce
    const unsigned char k[] = "000102030405060708090A0B0C0D0E0F";     // 128-bit key

    crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);

    printf("Ciphertext: %s", c);
    return 0;
}