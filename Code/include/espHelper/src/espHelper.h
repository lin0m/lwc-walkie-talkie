#include <string.h>
#include <stdio.h>
#include "hardware/uart.h"
#include "pico/stdlib.h"

bool waitUntilReady(char* currentString, const size_t length, uart_inst_t* uart_ID);
