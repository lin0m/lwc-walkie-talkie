#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "crypto_aead.h"
#include "espHelper.h"
#include "mic.h"
#include "../X3DH/ed25519/src/ed25519.h"
#include "../X3DH/sha/rfc6234/sha.h"


#define UART_ID uart1
#define BAUD_RATE 115200
#define UART_TX_PIN 4
#define UART_RX_PIN 5

void createClient()
{
    // initialize esp client
    initEsp();
    char currentString[256] = "";
    uart_puts(UART_ID, "AT+CWMODE=1\r\n");
    while (!waitUntilReady(currentString, 256, UART_ID))
    {
        uart_puts(UART_ID, "AT+CWMODE=1\r\n");
        printf(currentString);
    }
    printf("connecting to wifi");
    uart_puts(UART_ID, "AT+CWJAP=\"expressif\",\"1234567890\"\r\n");
    while (!waitUntilReady(currentString, 256, UART_ID))
    {
        uart_puts(UART_ID, "AT+CWJAP=\"expressif\",\"1234567890\"\r\n");
        printf(currentString);
    }
    printf("requesting ip info");
    uart_puts(UART_ID, "AT+CIPSTA?\r\n");
    while (!waitUntilReady(currentString, 256, UART_ID))
    {
        uart_puts(UART_ID, "AT+CIPSTA?\r\n");
        printf(currentString);
    }
    // change the ip based on the previous command output
    uart_puts(UART_ID, "AT+CIPSTART=\"TCP\",\"192.168.4.1\",2399\r\n");
    while (!waitUntilReady(currentString, 256, UART_ID))
    {
        uart_puts(UART_ID, "AT+CIPSTART=\"TCP\",\"192.168.4.1\",2399\r\n");
        printf(currentString);
    }
}
void encrypt(const unsigned char *m, unsigned long long mlen, const unsigned char *k);
int FetchPreKeyBundle(unsigned char *bob_id_public_key, unsigned char *bob_spk_public_key, unsigned char *bob_spk_signature, int message_type){};
void get_dh_output(unsigned char *bob_id_public_key, unsigned char *ephemeral_private_key, unsigned char *id_private_key,
                   unsigned char *bob_spk_public_key, unsigned char *dh_final);
void get_shared_key(unsigned char *dh_final, SHAversion whichSha, const unsigned char *salt_len, const unsigned char *info,
                    unsigned char *output_key, int okm_len);
int main(void)
{
    stdio_init_all();
    /*-------------------------------DEFINING VARIABLES FOR X3DH--------------------------*/
    unsigned char bob_id_public_key[32];  // bob's identity key
    unsigned char bob_spk_public_key[32]; // bob's signed prekey
    unsigned char bob_spk_signature[64];  // bob's signed prekey signature
    unsigned char id_public_key[32];      // alice identity public key
    unsigned char id_private_key[64];     // alice identity private key
    unsigned char seed[32];               // Seed to generate new keys
    unsigned char scalar[32];             // Scalar to add to modify key
    unsigned char ephemeral_public_key[32];
    unsigned char ephemeral_private_key[64];
    const unsigned char message[120]; // Message of the audio file to send - message size is still varying
    const int message_len = strlen((char *)message);
    unsigned char dh_final[96];         // To store the concatenation of the dh outputs
    unsigned char hex_hkdf_output[128]; // The 128-bit key for encryptig the audio

    // Creating Long-term keypair for Alice
    ed25519_create_seed(seed);
    ed25519_create_keypair(id_public_key, id_private_key, seed);

    /*----------------------------------CONNECT TO SERVER----------------------------------*/
    /*----------------------------------LISTEN TO SERVER----------------------------------*/
    // loop until message received
    // while (FetchPreKeyBundle(bob_id_public_key, bob_spk_public_key, bob_spk_signature, 2) == 1)
    // {
    //     /**
    //      * Waiting for server to send prekey bundle
    //      */
    //     continue;
    // }

    /*------------------------------------FINISH X3DH-------------------------------------*/
    if (ed25519_verify(bob_spk_signature, bob_id_public_key))
    {
        printf("valid signature\n");
    }
    else
    {
        printf("invalid signature\n");
        // Abort();
    }

    // After verification, generate Ephemeral Key pair
    ed25519_create_seed(seed);
    ed25519_create_keypair(ephemeral_private_key, ephemeral_public_key, seed);

    get_dh_output(bob_id_public_key, ephemeral_private_key, id_private_key, bob_spk_public_key, dh_final);
    get_shared_key(dh_final, SHA512, NULL, NULL, hex_hkdf_output, 128);
    /*--------------------------------WAIT FOR START BUTTON-------------------------------*/
    // loop until button pushed

    /*---------------LOOP "ENCRYPT AUDIO" & "SEND ENCRYPTED AUDIO" FOREVER----------------*/
    /*-----------------------------------ENCRYPT AUDIO------------------------------------*/
    // variables
    const size_t SAMPLE_FREQUENCY = 44100;
    const size_t BYTES_PER_SAMPLE = 4;
    const size_t SIZE_OF_RETURN = 2;
    const size_t HALF_SECOND_BUFFER = ((SAMPLE_FREQUENCY * BYTES_PER_SAMPLE) / 2);
    printf("initializing esp\n");
    for (size_t i = 0; i < 10; i++)
    {
        printf("%d\n", i);
        sleep_ms(1000);
    }
    
    // createClient();
    char sampleArr[(HALF_SECOND_BUFFER) + SIZE_OF_RETURN];
    PIO pio;
    uint sm;
    initMic(&pio, &sm);
    int32_t sample;
    absolute_time_t current = get_absolute_time();
    while (true)
    {
        current = get_absolute_time();
        for (uint64_t i = 0; i < (SAMPLE_FREQUENCY) / (size_t)2; i += 4)
        {
            sample = getSingleSampleBlocking(pio, sm);
            sampleArr[i] = (0xFF000000 & sample) >> 24;
            sampleArr[i + 1] = (0x00FF0000 & sample) >> 16;
            sampleArr[i + 2] = (0x0000FF00 & sample) >> 8;
            sampleArr[i + 3] = (0x000000FF & sample);
            printf("i is: %llu\n", i);
            if (!printTimer(&current, 1000*10)) {
                printf("sample is: %X\n", sample);
                current = get_absolute_time();
            }
        }
        char cipCommand[80];
        // send half a second buffer for now; later change it to fit in tinyJambu
        // sendCip(HALF_SECOND_BUFFER, cipCommand);
        // uart_puts(UART_ID, cipCommand);
        // data goes here:
        // uart_puts(UART_ID, strcat(sampleArr, "\r\n"));
    }

    const unsigned char m[] = {0x00};                                                                                           // plaintext
    unsigned long long mlen = sizeof(m);                                                                                        // plaintext length                                       // ciphertext
    const unsigned char k[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F}; // 128-bit key
    encrypt(m, mlen, hex_hkdf_output);

    /*--------------------------------SEND ENCRYPTED AUDIO--------------------------------*/
    return 0;
}

void encrypt(const unsigned char *m, unsigned long long mlen, const unsigned char *k)
{
    unsigned long long c_length = 80;                                                                      // ciphertext length
    unsigned char c[c_length];                                                                             // ciphertext
    unsigned long long *clen = &c_length;                                                                  // ciphertext length pointer
    const unsigned char ad[] = {0x00};                                                                     // associated data
    unsigned long long adlen = sizeof(ad);                                                                 // associated data length
    const unsigned char *nsec;                                                                             // secret message number
    const unsigned char npub[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B}; // public message number

    crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);

    printf("Ciphertext = ");
    for (int i = 0; i < c_length; i++)
    {
        printf("%02X", c[i]);
    }
}

void get_dh_output(unsigned char *bob_id_public_key, unsigned char *alice_ephemeral_private_key, unsigned char *alice_id_private_key,
                   unsigned char *bob_spk_public_key, unsigned char *dh_final)
{
    // DH outputs
    unsigned char dh1[32], dh2[32], dh3[32]; // DH exchanges - no opk so only 3 outputs

    ed25519_key_exchange(dh1, bob_spk_public_key, alice_id_private_key);

    // DH2 = DH(EKA, IKB)
    ed25519_key_exchange(dh2, bob_id_public_key, alice_ephemeral_private_key);

    // DH3 = DH(EKA, SPKB)
    ed25519_key_exchange(dh3, bob_spk_public_key, alice_ephemeral_private_key);

    // Concatenating dh outputs - because strcat, strncat and memcpy doesn't seem to work.
    for (int j = 0; j < 96; j++)
    {
        if (j < 32)
            dh_final[j] = dh1[j];
        if (j >= 32 && j < 64)
            dh_final[j] = dh2[j % 32];
        if (j >= 64)
            dh_final[j] = dh3[j % 32];
    }
}

void get_shared_key(unsigned char *dh_final, SHAversion whichSha, const unsigned char *salt, const unsigned char *info,
                    unsigned char *output_key, int okm_len)
{
    int salt_len; // The length of the salt value (a non-secret random value) (ignored if SALT==NULL)
    int info_len; // The length of optional context and application (ignored if info==NULL)
    int ikm_len;  // The length of the input keying material
    uint8_t okm_integer[okm_len];
    ikm_len = 96;

    if (salt == NULL)
        salt_len = 0;
    if (info == NULL)
        info_len = 0;

    if (hkdf(whichSha, salt, salt_len, dh_final, ikm_len, info, info_len, okm_integer, okm_len) == 0)
    {
        printf("HKDF is valid\n");
    }
    else
    {
        fprintf(stderr, "HKDF is invalid\n");
    }

    for (int i = 0; i < okm_len; i++)
    {
        output_key[i] = okm_integer[i];
        printf("%d\n", output_key[i]);
    }
}