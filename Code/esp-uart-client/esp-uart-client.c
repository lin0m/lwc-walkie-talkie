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
    uart_puts(UART_ID, "AT+CWMODE=1\r\n");
    while (!waitUntilReady(currentString, 256, UART_ID)) {
        uart_puts(UART_ID, "AT+CWMODE=1\r\n");
        printf(currentString);
    }
    sleep_ms(10000);
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
    uart_puts(UART_ID, "AT+CIPSTART=\"TCP\",\"192.168.4.1\",333\r\n");
    while (!waitUntilReady(currentString, 256, UART_ID)) {
        uart_puts(UART_ID, "AT+CIPSTART=\"TCP\",\"192.168.4.1\",333\r\n");
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
