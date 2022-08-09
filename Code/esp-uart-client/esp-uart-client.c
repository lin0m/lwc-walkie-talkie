/*  Project:        Audio Encryption Project
    Description:    Element 14 Pi-Fest challenge submission. System comprised of three Raspberry Pi Picos 
                    named Bob, Alice, and Server. System securely sends audio between Bob and Alice using 
                    a lightweight cryptographic algorithm. Keys are exchanged using X3DH. Bob receives audio 
                    as input, encrypts it, and sends it to the server. Alice receives the message, decrypts
                    it, and plays the audio.
    Documentation:  https://github.com/lin0m/lightweight-cryptography
    Date:           Aug 9th, 2022
    School:         Cal Poly Pomona 
    Group:          Dr. Aly's Research Team
    Members:        Melvin Relf
                    Russell Hua
                    Alexander Ea
                    Shahzman Saqib
                    Muhammed Kamil
                    Patricia Ankunda
                    Lino Mercado-Esquivias
*/

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "espHelper.h"
#define UART_ID uart1
#define BAUD_RATE 115200
#define UART_ID1 uart0

#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define UART_TX_PIN1 0
#define UART_RX_PIN1 1

int main()
{
    stdio_init_all();
    uart_init(UART_ID, BAUD_RATE);
    uart_init(UART_ID1, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_TX_PIN1, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN1, GPIO_FUNC_UART);
    char currentString[256] = "";
    // char temp[80] = "";
    for (size_t i = 0; i < 10; i++)
    {
        printf("%d\n", i);
        sleep_ms(1000);
    }
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
    printf(currentString);
    // change the ip based on the previous command output
    uart_puts(UART_ID, "AT+CIPSTART=\"TCP\",\"192.168.4.1\",2339\r\n");
    while (!waitUntilReady(currentString, 256, UART_ID)) {
        uart_puts(UART_ID, "AT+CIPSTART=\"TCP\",\"192.168.4.1\",2339\r\n");
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
    
    return 0;
}
