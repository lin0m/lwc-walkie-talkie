#include <stdio.h>
#include <string.h>
#ifdef USE_PICO
#include "pico/stdlib.h"
#include "hardware/pio.h"
#endif
// #include "crypto_aead.h"
//#include "espHelper.h"
//#include "mic.h"
// #include "../X3DH/ed25519/src/ed25519.h"
// #include "../X3DH/rfc6234/sha.h"

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "mic.pio.h"
//#include "/home/nasa/lightweight-cryptography/Code/mic/build/dac.pio.h"



#define UART_ID uart1
#define BAUD_RATE 115200
#define UART_TX_PIN 4
#define UART_RX_PIN 5
bool waitUntilReady(char *currentString, const size_t length, uart_inst_t *uart_ID)
{
    char input = ' ';
    strcpy(currentString, "");
    char *resultOK = NULL;
    char *resultERROR = NULL;
    while (resultOK == NULL && resultERROR == NULL)
    {
        resultOK = strstr(currentString, "OK");
        resultERROR = strstr(currentString, "ERROR");
        input = uart_getc(uart_ID);
        strncat(currentString, &input, 1);
        // printf(currentString);
        // if input is too long, reset the string
        if (strlen(currentString) >= length)
        {
            strcpy(currentString, "");
        }
    }
    if (resultERROR != NULL)
    {
        // strcpy(currentString, "");
        return false;
    }
    else if (resultOK != NULL)
    {
        // strcpy(currentString, "");
        return true;
    }
    else
    {
        return false;
    }
}
void createClient()
{
    // initialize esp client
  //  initEsp();
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
    uart_puts(UART_ID, "AT+CIPSEND=4\r\n");
    while (!waitUntilReady(currentString, 256, UART_ID)) {
        uart_puts(UART_ID, "AT+CIPSEND=4\r\n");
        printf(currentString);
    }
    uart_puts(UART_ID, "test\r\n");
    // while (!waitUntilReady(currentString, 256, UART_ID)) {
    //     uart_puts(UART_ID, "AT+CIPSEND=4\r\n");
    // }
}
// void encrypt(const unsigned char *m, unsigned long long mlen, const unsigned char *k);
// int FetchPreKeyBundle(unsigned char *bob_id_public_key, unsigned char *bob_spk_public_key, unsigned char *bob_spk_signature, int message_type){};
// void get_dh_output(unsigned char *bob_id_public_key, unsigned char *ephemeral_private_key, unsigned char *id_private_key,
//                    unsigned char *bob_spk_public_key, unsigned char *dh_final);
// void get_shared_key(unsigned char *dh_final, SHAversion whichSha, const unsigned char *salt_len, const unsigned char *info,
//                     unsigned char *output_key, int okm_len);



void sendTimer(const int16_t MAX_VALUE_I2S, const double C_PERIOD_MS, PIO pio, uint sm, absolute_time_t* current)
{
    // pio_sm_put_blocking(pio, sm, leftRightData);
    // sleep_ms(C_PERIOD_MS / 2);
    // absolute_time_t pioTime;
    uint32_t leftRightData;
    int32_t right;
    int32_t left;
    if (absolute_time_diff_us(*current, get_absolute_time()) < (int64_t)(((C_PERIOD_MS * 1000) / 2)))
    {
        right = (MAX_VALUE_I2S << 16u) & 0xFFFF0000;
        left = MAX_VALUE_I2S & 0x0000FFFF;
        leftRightData = (uint32_t)right | (uint32_t)left;
        // printf("Sending positive %X\n", leftRightData);
        // pioTime = get_absolute_time();
        pio_sm_put_blocking(pio, sm, leftRightData);
        // printf("Pio time is %lld\n", (absolute_time_diff_us(pioTime, get_absolute_time())));
        // printf("Time used is: %lld\n", (absolute_time_diff_us(current, get_absolute_time())));
    } else if (absolute_time_diff_us(*current, get_absolute_time()) < (int64_t)(((C_PERIOD_MS * 1000)))) {
        right = -((MAX_VALUE_I2S << 16u) & 0xFFFF0000);
        left = -MAX_VALUE_I2S & 0x0000FFFF;
        leftRightData = (uint32_t)right | (uint32_t)left;
        // pio_sm_put_blocking(pio, sm, leftRightData);
        // sleep_ms(C_PERIOD_MS / 2);
        pio_sm_put_blocking(pio, sm, leftRightData);
        // printf("Sending negative %X\n", leftRightData);
        // pioTime = get_absolute_time();
        // printf("Pio time is %lld\n", (absolute_time_diff_us(pioTime, get_absolute_time())));
        // printf("Time used is: %lld\n", (absolute_time_diff_us(current, get_absolute_time())));
    } else {
        *current = get_absolute_time();
    }
}
bool printTimer(absolute_time_t* current, int64_t waitTime_us) {
    if (absolute_time_diff_us(*current, get_absolute_time()) > waitTime_us)
    {
        return true;
    } else {
        return false;
    }
}
void initMic(PIO* pio, uint *sm) {
    *pio = pio0;
    uint offset = pio_add_program(*pio, &mic_program);
    *sm = pio_claim_unused_sm(*pio, true);
    mic_program_init(*pio, *sm, offset, 18, 26);
    pio_sm_set_enabled(*pio, *sm, false);
    pio_sm_clear_fifos(*pio, *sm);
    pio_sm_restart(*pio, *sm);
    pio_sm_set_enabled(*pio, *sm, true);
}
int32_t getSingleSampleBlocking(PIO pio, uint sm) {
    int32_t lrDataFull = pio_sm_get_blocking(pio, sm);
    int32_t lrData = lrDataFull & 0xFFFF0000;
    return lrData;
}






int main(void)
{
       stdio_init_all();

  createClient();
     char currentString[44100 / 2];
     char singleSample[6];
    int32_t sample;
    for (size_t i = 0; i < 5; i++)
    {
        printf("%d\n", i);
        sleep_ms(1000);
    }
    PIO pio;
    uint sm;
    initMic(&pio, &sm);

    // PIO pio_dac = pio1;
    // uint offset_dac = pio_add_program(pio_dac, &dac_program);
    // uint sm_dac = pio_claim_unused_sm(pio_dac, true);
    // dac_program_init(pio_dac, sm_dac, offset_dac, 13, 6);
    int32_t lrDataFull = 0;
    int32_t lrData = 0;

  absolute_time_t printCurrent = get_absolute_time();
    // while (true)
    // {
        // sendTimer(MAX_VALUE_I2S, C_PERIOD_MS, pio_dac, sm_dac, &current);
        lrData = getSingleSampleBlocking(pio, sm);
        // pio_sm_put_blocking(pio_dac, sm_dac, lrData / 1024);
        if (printTimer(&printCurrent, 0*1000)) {
            // printf("data is: %d\t%08X\n", lrData, lrData);
            printCurrent = get_absolute_time();
        }
    // }


    // /*-------------------------------DEFINING VARIABLES FOR X3DH--------------------------*/
    // unsigned char bob_id_public_key[32];  // bob's identity key
    // unsigned char bob_spk_public_key[32]; // bob's signed prekey
    // unsigned char bob_spk_signature[64];  // bob's signed prekey signature
    // unsigned char id_public_key[32];      // alice identity public key
    // unsigned char id_private_key[64];     // alice identity private key
    // unsigned char seed[32];               // Seed to generate new keys
    // unsigned char scalar[32];             // Scalar to add to modify key
    // unsigned char ephemeral_public_key[32];
    // unsigned char ephemeral_private_key[64];
    // const unsigned char message[120]; // Message of the audio file to send - message size is still varying
    // const int message_len = strlen((char *)message);
    // unsigned char dh_final[96];         // To store the concatenation of the dh outputs
    // unsigned char hex_hkdf_output[128]; // The 128-bit key for encryptig the audio

    // // Creating Long-term keypair for Alice
    // ed25519_create_seed(seed);
    // ed25519_create_keypair(id_public_key, id_private_key, seed);

/*----------------------------------CONNECT TO SERVER----------------------------------*/
// stdio_init_all();
//#ifdef USE_PICO
//#endif

//#ifdef USE_PICO
//#endif
    /*----------------------------------LISTEN TO SERVER----------------------------------*/
    // loop until message received
    // while (FetchPreKeyBundle(bob_id_public_key, bob_spk_public_key, bob_spk_signature, 2) == 1)
    // {
    //     /**
    //      * Waiting for server to send prekey bundle
    //      */
    //     continue;
    // }

    // /*------------------------------------FINISH X3DH-------------------------------------*/
    // if (ed25519_verify(bob_spk_signature, bob_id_public_key))
    // {
    //     printf("valid signature\n");
    // }
    // else
    // {
    //     printf("invalid signature\n");
    //     // Abort();
    // }

    // // After verification, generate Ephemeral Key pair
    // ed25519_create_seed(seed);
    // ed25519_create_keypair(ephemeral_private_key, ephemeral_public_key, seed);

    // get_dh_output(bob_id_public_key, ephemeral_private_key, id_private_key, bob_spk_public_key, dh_final);
    // get_shared_key(dh_final, SHA512, NULL, NULL, hex_hkdf_output, 128);
    // /*--------------------------------WAIT FOR START BUTTON-------------------------------*/
    // loop until button pushed

    /*---------------LOOP "ENCRYPT AUDIO" & "SEND ENCRYPTED AUDIO" FOREVER----------------*/
    /*-----------------------------------ENCRYPT AUDIO------------------------------------*/
    // variables
   
  //  stdio_init_all();
  /*  for (int i = 0; i < 10; i++)
    {
        printf("%d\n", i);
        sleep_ms(1000);
    }*/
 /*   printf("starting pio\n");
    PIO pio;
    uint sm;
    initMic(&pio, &sm);
    printf("starting loop\n");
    while (true)
    {
        printf("in loop getting sample\n");

         for (size_t i = 0; i < (44100 - 4) / 2; i += 5)
         {
        //while(true)
        //{
            sample = pio_sm_get_blocking(pio, sm);
            sleep_ms(1000);
            
            // sleep_ms(1000);

            printf("sample is: %x\n", sample);
        //}
         }
        */
        int i = 0;
        sample = getSingleSampleBlocking(pio, sm);
        printf("sample: %08X\n", sample);
        singleSample[i] = (0xFF000000 & sample) >> 24;
        singleSample[i + 1] = (0x00FF0000 & sample) >> 16;
        singleSample[i + 2] = (0x0000FF00 & sample) >> 8;
        singleSample[i + 3] = (0x000000FF & sample);
        
         char atSend[80] = "AT+CIPSEND=";
         // send half a second buffer for now; later change it to fit in tinyJambu
         size_t amount = 4;
         char amountChar[80];
         sprintf(amountChar, "%llu", amount);
         strcat(atSend, amountChar);
         strcat(atSend, "\r\n");
         printf("\nWe are here");
         uart_puts(UART_ID, atSend);
         printf("%s\n", atSend);
     for (size_t i = 0; i < 10; i++)
     {
         printf("%d\n", i);
         sleep_ms(1000);
     }
    /*
         while (!waitUntilReady(currentString, 2048, UART_ID))
         {
             printf("\nIn while loop");
             uart_puts(UART_ID, atSend);
             printf(currentString);
         }
         */

    //     // data goes here:

         uart_puts(UART_ID, strcat(singleSample, "\r\n"));
 //   }
  //  printf("Leaving loop\n");
    // const unsigned char m[] = {0x00};                                                                                           // plaintext
    // unsigned long long mlen = sizeof(m);                                                                                        // plaintext length                                       // ciphertext
    // const unsigned char k[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F}; // 128-bit key
    // encrypt(m, mlen, hex_hkdf_output);

    /*--------------------------------SEND ENCRYPTED AUDIO--------------------------------*/
    return 0;
}

// void encrypt(const unsigned char *m, unsigned long long mlen, const unsigned char *k)
// {
//     unsigned long long c_length = 80;                                                                      // ciphertext length
//     unsigned char c[c_length];                                                                             // ciphertext
//     unsigned long long *clen = &c_length;                                                                  // ciphertext length pointer
//     const unsigned char ad[] = {0x00};                                                                     // associated data
//     unsigned long long adlen = sizeof(ad);                                                                 // associated data length
//     const unsigned char *nsec;                                                                             // secret message number
//     const unsigned char npub[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B}; // public message number

//     crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);

//     printf("Ciphertext = ");
//     for (int i = 0; i < c_length; i++)
//     {
//         printf("%02X", c[i]);
//     }
// }

// void get_dh_output(unsigned char *bob_id_public_key, unsigned char *ephemeral_private_key, unsigned char *id_private_key,
//                    unsigned char *bob_spk_public_key, unsigned char *dh_final)
// {
//     // DH outputs
//     unsigned char dh1[32], dh2[32], dh3[32]; // DH exchanges - no opk so only 3 outputs

//     // DH1 = DH(IKA, SPKB)
//     ed25519_key_exchange(dh1, bob_id_public_key, id_private_key);

//     // DH2 = DH(EKA, IKB)
//     ed25519_key_exchange(dh2, bob_id_public_key, ephemeral_private_key);

//     // DH3 = DH(EKA, SPKB)
//     ed25519_key_exchange(dh3, bob_spk_public_key, ephemeral_private_key);

//     // Concatenating dh outputs
//     strcat(dh_final, dh1);
//     strcat(dh_final, dh2);
//     strcat(dh_final, dh3);
// }

// void get_shared_key(unsigned char *dh_final, SHAversion whichSha, const unsigned char *salt, const unsigned char *info,
//                     unsigned char *output_key, int okm_len)
// {
//     int salt_len; // The length of the salt value (a non-secret random value) (ignored if SALT==NULL)
//     int info_len; // The length of optional context and application (ignored if info==NULL)
//     int ikm_len;  // The length of the input keying material
//     uint8_t okm_integer[okm_len];
//     ikm_len = 96;

//     if (salt == NULL)
//         salt_len = 0;
//     if (info == NULL)
//         info_len = 0;

//     if (hkdf(whichSha, salt, salt_len, dh_final, ikm_len, info, info_len, okm_integer, okm_len) == 0)
//     {
//         printf("HKDF is valid\n");
//     }
//     else
//     {
//         fprintf(stderr, "HKDF is invalid\n");
//     }

//     for (int i = 0; i < okm_len; i++)
//     {
//         output_key[i] = okm_integer[i];
//         printf("%d\n", output_key[i]);
//     }
// }
