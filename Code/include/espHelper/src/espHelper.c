#include "espHelper.h"

/**
 * @brief searches for a string once
 * @note the uart_ID paramter can be replaced with a char if the caller provides the character from uart
 * @param key string to search
 * @param currentString string containing input from uart, will contain the key being searched
 * @param length capacity of uart string input
 * @param uart_ID uart instance to use
 * @return true - the key was found
 * @return false - the key wasn't found
 */
bool searchUARTOnce(char *key, char *currentString, size_t length, uart_inst_t *uart_ID)
{
    char input;
    char *result = NULL;
    result = strstr(currentString, key);
    if (result == NULL)
    {
        input = uart_getc(uart_ID);
        strncat(currentString, &input, 1);
        // printf(currentString);
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * @brief searches for a string once
 * @note this function removes the requirement on pico uart; append a new character outside the function
 * This also allows searching multiple keys
 * @param key string to search
 * @param currentString string containing input from uart, will contain the key being searched
 * @return true - the key was found
 */
bool searchStringOnce(char *key, char *currentString)
{
    if (strstr(currentString, key) == NULL)
    {
        // printf(currentString);
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * @brief send a command, then loop this with the same command until it exits, then send another command
 * @note TODO the code should use searchStringOnce
 * @param currentString string to add characters to and check
 * @param length
 * @return true no error
 * @return false error
 */
bool waitUntilReady(char *currentString, const size_t length, uart_inst_t *uart_ID)
{
    char input = ' ';
    strcpy(currentString, "");
    char *resultOK = NULL;
    char *resultERROR = NULL;
    while (resultOK == NULL && resultERROR == NULL)
    {
        resultOK = strstr(currentString, "OK");
        resultERROR = strstr(currentString, "ERROR");
        input = uart_getc(uart_ID);
        strncat(currentString, &input, 1);
        // printf(currentString);
        // if input is too long, reset the string
        if (strlen(currentString) >= length)
        {
            strcpy(currentString, "");
        }
    }
    if (resultERROR != NULL)
    {
        // strcpy(currentString, "");
        return false;
    }
    else if (resultOK != NULL)
    {
        // strcpy(currentString, "");
        return true;
    }
    else
    {
        return false;
    }
}
/**
 * @brief parses and gets the data from esp tcp server and returns the link ID
 *
 * @param uart_ID uart instance to get from
 * @param result the data from esp tcp server; cleared when function starts This must be large enough to fit data and a null character at the end
 * The function adds a null character at the end.
 * @param resultCapacity capacity of result; unused for now
 * @return int - the link id, -1 if error
 */
int getTCPEsp(uart_inst_t *uart_ID, char *result, size_t resultCapacity)
{
    char input = ' ';
    char linkID[80];
    char amount[80];
    char *tempStr = NULL;
    strcpy(result, "");
    strcpy(amount, "");
    strcpy(linkID, "");
    input = uart_getc(uart_ID);
    while (input != ',')
    {
        input = uart_getc(uart_ID);
        // printf("IPD: %c", input);
    }
    input = uart_getc(uart_ID);
    while (input != ',')
    {
        strncat(linkID, &input, 1);
        // printf("linkID: %s", linkID);
        input = uart_getc(uart_ID);
    }
    input = uart_getc(uart_ID);
    while (input != ':')
    {
        strncat(amount, &input, 1);
        // printf("amount: %s", amount);
        input = uart_getc(uart_ID);
    }
    // TODO no bounds checking here, there might be problems later
    printf("amount of data is: %d", atoi(amount));
    printf("result: ");
    for (size_t i = 0; i < atoi(amount); i++)
    {
        printf("i is: %d\n", i);
        input = uart_getc(uart_ID);
        result[i] = input;
        printf("%c", input);
    }
    result[atoi(amount)] = '\0';
    return atoi(linkID);
}
void initEsp()
{
    stdio_init_all();
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}
/**
 * @brief constructs the cip command based on the number of bytes to send
 *
 * @param numberOfBytes
 * @param command
 */
void sendCip(const uint64_t numberOfBytes, char *command)
{
    // strcpy(command, "");
    strcpy(command, "AT+CIPSEND=");
    // printf("start of command is: %s\n", command);
    char charBytesToSend[80];
    sprintf(charBytesToSend, "%llu", numberOfBytes);
    // printf("bytes being sent is: %s\n", charBytesToSend);
    strcat(command, charBytesToSend);
    // printf("after adding byte amount: %s\n", command);
    strcat(command, "\r\n");
    // printf("after adding returns: %s", command);
}