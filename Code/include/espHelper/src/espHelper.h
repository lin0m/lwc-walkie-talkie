#include <string.h>
#include <stdio.h>
#include "hardware/uart.h"
#include "pico/stdlib.h"
#include "stdlib.h"
#define UART_ID uart1
#define BAUD_RATE 115200
#define UART_TX_PIN 4
#define UART_RX_PIN 5
bool waitUntilReady(char* currentString, const size_t length, uart_inst_t* uart_ID);
bool getCharAmount(size_t amount, uart_inst_t* uart_ID, char* result, size_t resultCapacity);
int getTCPEsp(uart_inst_t* uart_ID, char* result, size_t resultCapacity, size_t* charAmount);
bool searchUARTOnce(char* key, char* currentString, size_t length, uart_inst_t* uart_ID);
bool searchStringOnce(char* key, char* currentString);
void initEsp();
void sendCip(const uint64_t numberOfBytes, char* command);