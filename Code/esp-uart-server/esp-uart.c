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
/**
 * @brief send a command, then loop this with the same command until it exits, then send another command
 * 
 * @param currentString 
 * @param length 
 * @return true no error
 * @return false error
 */
bool waitUntilReady(char* currentString, const size_t length) {
    char input = ' ';
    strcpy(currentString, "");
    char* resultOK = NULL;
    char* resultERROR = NULL;
    while (resultOK == NULL && resultERROR == NULL) {
        resultOK = strstr(currentString, "OK");
        resultERROR = strstr(currentString, "ERROR");
        input = uart_getc(UART_ID);
        strncat(currentString, &input, 1);
        // if input is too long, reset the string
        if (strlen(currentString) >= length) {
            strcpy(currentString, "");
        }
    }
    if (resultERROR != NULL) {
        return false;
    } else if (resultOK != NULL) {
        return true;
    }
}
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
bool ready(char* currentString, const size_t length){
    currentString = "";
    uart_puts(UART_ID1, "Blank string:\n");
    uart_puts(UART_ID1, currentString);
    
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
    uart_puts(UART_ID, "AT+CWMODE=2\r\n");
    char currentString[80] = "test";
    printf(currentString);
    while (!waitUntilReady(currentString, 80)) {
        uart_puts(UART_ID, "AT+CWMODE=2\r\n");
        uart_puts(UART_ID1, currentString);
    }
    uart_puts(UART_ID, "AT+CIPMUX=1\r\n");
    while (!waitUntilReady(currentString, 80)) {
        uart_puts(UART_ID, "AT+CIPMUX=1\r\n");
        uart_puts(UART_ID1, currentString);
    }
    uart_puts(UART_ID, "AT+CWSAP=\"expressif\",\"1234567890\",5,3\r\n");
    while (!waitUntilReady(currentString, 80)) {
        uart_puts(UART_ID, "AT+CWSAP=\"expressif\",\"1234567890\",5,3\r\n");
        uart_puts(UART_ID1, currentString);
    }
    sleep_ms(10000);
    uart_puts(UART_ID, "AT+CIPSERVER=1\r\n");
    while (!waitUntilReady(currentString, 80)) {
        uart_puts(UART_ID1, currentString);
        printf(currentString);
    }
    return 0;
}
