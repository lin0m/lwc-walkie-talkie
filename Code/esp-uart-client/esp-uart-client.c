#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART_ID uart1
#define BAUD_RATE 115200
#define UART_ID1 uart0

#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define UART_TX_PIN1 0
#define UART_RX_PIN1 1

bool waitUntilReady(char* currentString, const size_t length) {
    char input = ' ';
    currentString = "";
    char* resultOK = NULL;
    char* resultERROR = NULL;
    while (resultOK == NULL && resultERROR == NULL) {
        resultOK = strstr(currentString, "OK");
        resultERROR = strstr(currentString, "ERROR");
        input = uart_getc(UART_ID);
        // if input is too long, reset the string
        strncat(currentString, &input, 1);
        if (sizeof(currentString) >= length) {
            currentString = "";
        }
    }
    if (resultERROR != NULL) {
        return false;
    } else if (resultOK != NULL) {
        return true;
    }
}

int main()
{
    stdio_init_all();
    uart_init(UART_ID, BAUD_RATE);
    uart_init(UART_ID1, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_TX_PIN1, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN1, GPIO_FUNC_UART);
    uart_puts(UART_ID, "AT+CWMODE=1\r\n");
    const size_t kcurrentString = 80;
    char currentString[kcurrentString];
    while (!waitUntilReady(currentString, 80)) {
        uart_puts(UART_ID, "AT+CWJAP=\"espressif\",\"1234567890\"\r\n");
    }
    while (!waitUntilReady(currentString, 80)) {
        uart_puts(UART_ID, "AT+CIPSTA?\r\n");
    }
    while (!waitUntilReady(currentString, 80)) {
        uart_puts(UART_ID, "AT+CIPSTART=\"TCP\",\"192.168.3.102\",8080\r\n");
    }

    while (!waitUntilReady(currentString, 80)) {
        uart_puts(UART_ID, "AT+CIPSEND=4\r\n");
    }
    uart_puts(UART_ID, "test\r\n");
    // while (!waitUntilReady(currentString, 80)) {
    //     uart_puts(UART_ID, "AT+CIPSEND=4\r\n");
    // }
    
    return 0;
}
