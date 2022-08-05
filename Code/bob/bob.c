#include <stdio.h>
#include <string.h>
#ifdef USE_PICO
    #include "pico/stdlib.h"
    #include "hardware/pio.h"
#endif
#include "crypto_aead.h"
#include "espHelper.h"
#include "mic.pio.h"

#define UART_ID uart1
#define BAUD_RATE 115200
#define UART_TX_PIN 4
#define UART_RX_PIN 5


int main(void){
    // stdio_init_all();
    #ifdef USE_PICO
        stdio_init_all();
        uart_init(UART_ID, BAUD_RATE);
        gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
        gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    #endif

    /*
    Count = 35
    Key = 000102030405060708090A0B0C0D0E0F
    Nonce = 000102030405060708090A0B
    PT = 00
    AD = 00
    CT = A175D5B5C1EE4A0FA1
    */

    char currentString[256] = "";
    #ifdef USE_PICO
        // initialize esp client
        uart_puts(UART_ID, "AT+CWMODE=1\r\n");
        while (!waitUntilReady(currentString, 256, UART_ID)) {
            uart_puts(UART_ID, "AT+CWMODE=1\r\n");
            printf(currentString);
        }
        printf("connecting to wifi");
        uart_puts(UART_ID, "AT+CWJAP=\"expressif\",\"1234567890\"\r\n");
        while (!waitUntilReady(currentString, 256, UART_ID)) {
            uart_puts(UART_ID, "AT+CWJAP=\"expressif\",\"1234567890\"\r\n");
            printf(currentString);
        }
        printf("requesting ip info");
        uart_puts(UART_ID, "AT+CIPSTA?\r\n");
        while (!waitUntilReady(currentString, 256, UART_ID)) {
            uart_puts(UART_ID, "AT+CIPSTA?\r\n");
            printf(currentString);
        }
        // change the ip based on the previous command output
        uart_puts(UART_ID, "AT+CIPSTART=\"TCP\",\"192.168.4.1\",2399\r\n");
        while (!waitUntilReady(currentString, 256, UART_ID)) {
            uart_puts(UART_ID, "AT+CIPSTART=\"TCP\",\"192.168.4.1\",2399\r\n");
            printf(currentString);
        }
        char atSend[80] = "AT+CIPSEND=";
        size_t amount = 0;
        char amountChar[80];
        sprintf(amountChar, "%llu", amount);
        strcat(atSend, amountChar);
        strcat(atSend, "\r\n");
        uart_puts(UART_ID, atSend);
        while (!waitUntilReady(currentString, 256, UART_ID)) {
            uart_puts(UART_ID, atSend);
            printf(currentString);
        }
        uart_puts(UART_ID, "test\r\n");
    #endif
    // variables
    unsigned long long c_length = 80;
    unsigned char c[c_length];           // ciphertext
    unsigned long long *clen = &c_length;   // ciphertext length
    // unsigned long long m_length = 80;
    unsigned char m[] = {0x00};     // plaintext

    unsigned long long mlen = sizeof(m);    // plaintext length
    const unsigned char ad[] = {0x00};    // associated data
    unsigned long long adlen = sizeof(ad);   // associated data length
    unsigned char *nsec;  // nonce
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
    // decrypt
    crypto_aead_decrypt(m, &mlen, nsec, c, *clen, ad, adlen, npub, k);

    // print
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
    printf("Plaintext = ");
    for (int i = 0; i < mlen; i++){
        printf("%02X", m[i]);
    }
    printf("\n");

    return 0;
}