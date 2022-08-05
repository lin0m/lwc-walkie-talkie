#include <string.h>
#include <stdio.h>
#include "hardware/uart.h"
#include "pico/stdlib.h"
#include "stdlib.h"

bool waitUntilReady(char* currentString, const size_t length, uart_inst_t* uart_ID);
bool getCharAmount(size_t amount, uart_inst_t* uart_ID, char* result, size_t resultCapacity);
int getTCPEsp(uart_inst_t* uart_ID, char* result, size_t resultCapacity);
bool searchUARTOnce(char* key, char* currentString, size_t length, uart_inst_t* uart_ID);
bool searchStringOnce(char* key, char* currentString);