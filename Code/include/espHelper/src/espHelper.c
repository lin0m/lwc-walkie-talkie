#include "espHelper.h"
/**
 * @brief send a command, then loop this with the same command until it exits, then send another command
 * 
 * @param currentString 
 * @param length 
 * @return true no error
 * @return false error
 */

bool waitUntilReady(char* currentString, const size_t length, uart_inst_t* uart_ID) {
    char input = ' ';
    strcpy(currentString, "");
    char* resultOK = NULL;
    char* resultERROR = NULL;
    while (resultOK == NULL && resultERROR == NULL) {
        resultOK = strstr(currentString, "OK");
        resultERROR = strstr(currentString, "ERROR");
        input = uart_getc(uart_ID);
        strncat(currentString, &input, 1);
        // printf(currentString);
        // if input is too long, reset the string
        if (strlen(currentString) >= length) {
            strcpy(currentString, "");
        }
    }
    if (resultERROR != NULL) {
        // strcpy(currentString, "");
        return false;
    } else if (resultOK != NULL) {
        // strcpy(currentString, "");
        return true;
    } else {
        return false;
    }
}
