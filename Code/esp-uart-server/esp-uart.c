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

bool waitUntilReady(char* currentString, const size_t length) {
    char input = ' ';
    currentString = "";
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
// /**
//  * @brief 
//  * 
//  * @param input 
//  * @param keys 
//  * @param amount 
//  * @return char* 
//  */
// char* searchForStrings(char* input, char** keys, size_t amount) {

// }
int main()
{
    stdio_init_all();
    uart_init(UART_ID, BAUD_RATE);
    uart_init(UART_ID1, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_TX_PIN1, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN1, GPIO_FUNC_UART);
    uart_puts(UART_ID, "AT+CWMODE=2\r\n");
    char currentString[80] = "";
    // while (!waitUntilReady()) {
    //     uart_puts(UART_ID, "AT+CWMODE=1\r\n");
    // }
    while (!waitUntilReady(currentString, 80)) {
        uart_puts(UART_ID, "AT+CIPMUX=1\r\n");
    }
    sleep_ms(10000);
    while (!waitUntilReady(currentString, 80)) {
        uart_puts(UART_ID1, "AT+CWSAP=\"expressif\",\"1234567890\",5,3\r\n");
        printf(currentString);
    }
    while (!waitUntilReady(currentString, 80)) {
        uart_puts(UART_ID, "AT+CIPSERVER=1\r\n");
    }
    while (!waitUntilReady(currentString, 80)) {
        uart_puts(UART_ID, "AT+CIPSEND=0,4\r\n");
    }
    uart_puts(UART_ID, "test\r\n");
    sleep_ms(1000);
    return 0;
}
