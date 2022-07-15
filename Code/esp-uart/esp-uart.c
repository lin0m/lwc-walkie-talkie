#include <stdio.h>
#include <string.h>
#include "hardware/uart.h"
#include "pico/stdlib.h"

#define UART_ID uart1
#define BAUD_RATE 115200
#define UART_ID1 uart0

#define UART_TX_PIN 4
#define UART_RX_PIN 5
#define UART_TX_PIN1 0
#define UART_RX_PIN1 1

bool waitUntilReady() {
    char input = ' ';
    char currentString[40] = "";
    char* resultOK = NULL;
    char* resultERROR = NULL;
    while (resultOK == NULL && resultERROR == NULL) {
        resultOK = strstr(currentString, "OK");
        resultERROR = strstr(currentString, "ERROR");
        input = uart_getc(UART_ID);
        strncat(currentString, &input, 1);
    }
    if (resultERROR != NULL) {
        return false;
    } else if (resultOK != NULL) {
        return true;
    }
}
char* searchForStrings(char* keys, size_t amount) {

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
    while (!waitUntilReady()) {
        uart_puts(UART_ID, "AT+CWMODE=1\r\n");
    }
    while (!waitUntilReady()) {
        uart_puts(UART_ID, "AT+CWJAP=\"espressif\",\"1234567890\"\r\n");
    }
    while (!waitUntilReady()) {
        uart_puts(UART_ID1, "AT+CIPSTA?\r\n");
    }
    while (!waitUntilReady()) {
        uart_puts(UART_ID1, "AT+CIPSTA?\r\n");
    }
    while (!waitUntilReady()) {
        uart_puts(UART_ID, "AT+CIPSTART=\"TCP\",\"192.168.3.102\",8080\r\n");
    }
    while (!waitUntilReady()) {
        uart_puts(UART_ID, "AT+CIPSEND=4\r\n");
    }
    sleep_ms(1000);
    return 0;
}
