#include <stdio.h>
#include <string.h>
#include "hardware/uart.h"
#include "pico/stdlib.h"
#include "espHelper.h"

#define UART_ID uart1
#define BAUD_RATE 115200

#define UART_TX_PIN 4
#define UART_RX_PIN 5

// char* searchForAllStrings(char* input, char** keys, size_t amount) {
//     char* result = NULL;
//     for (size_t i = 0; i < amount; i++)
//     {
//         strstr(input, keys[i]);
//         if (result == NULL) {
//             return NULL;
//         }
//     }
//     return result;
// }

int main()
{
    stdio_init_all();
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_puts(UART_ID, "AT+CWMODE=2\r\n");
    char currentString[256] = "test";
    for (size_t i = 0; i < 10; i++)
    {
        printf("%d\n", i);
        sleep_ms(1000);
    }
    printf("setting ap mode\n");
    while (!waitUntilReady(currentString, 256, UART_ID))
    {
        uart_puts(UART_ID, "AT+CWMODE=2\r\n");
    }
    printf("%s\n", currentString);
    printf("enabling multiple connections\n");
    uart_puts(UART_ID, "AT+CIPMUX=1\r\n");
    while (!waitUntilReady(currentString, 256, UART_ID))
    {
        printf("%s\n", currentString);
        uart_puts(UART_ID, "AT+CIPMUX=1\r\n");
    }
    printf("%s\n", currentString);
    printf("setting wifi ssid and password\n");
    uart_puts(UART_ID, "AT+CWSAP=\"expressif\",\"1234567890\",5,3\r\n");
    while (!waitUntilReady(currentString, 256, UART_ID))
    {
        uart_puts(UART_ID, "AT+CWSAP=\"expressif\",\"1234567890\",5,3\r\n");
    }
    printf("%s\n", currentString);
    for (size_t i = 0; i < 10; i++)
    {
        printf("%d\n", i);
        sleep_ms(1000);
    }
    printf("enabling tcpip server\n");
    uart_puts(UART_ID, "AT+CIPSERVER=1,2399\r\n");
    while (!waitUntilReady(currentString, 256, UART_ID))
    {
        uart_puts(UART_ID, "AT+CIPSERVER=1,2399\r\n");
        printf(currentString);
    }
    printf("%s\n", currentString);
    printf("getting ip information\n");
    uart_puts(UART_ID, "AT+CIPAP?\r\n");
    while (!waitUntilReady(currentString, 256, UART_ID))
    {
        uart_puts(UART_ID, "AT+CIPAP?\r\n");
        printf(currentString);
    }
    printf("%s\n", currentString);
    printf("waiting for connect\n");
    strcpy(currentString, "");
    char input = uart_getc(UART_ID);
    while (!searchStringOnce("CONNECT", currentString))
    {
        strncat(currentString, &input, 1);
        input = uart_getc(UART_ID);
    }
    strcpy(currentString, "");
    printf("Connected successfully");
    while (true)
    {
        getTCPEsp(UART_ID, currentString, 256);
        printf("%s", currentString);
    }
    return 0;
}
