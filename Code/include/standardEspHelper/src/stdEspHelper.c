#include "stdEspHelper.h"
// TODO: change all int errors to enums to make them clearer
/**
 * @brief searches a string for substring
 * @note this is created to simplify searching for a substring
 * @param key The string to search for
 * @param currentString The string to search in
 * @return true - the string was found
 * @return false - the string wasn't found
 */
bool searchStringOnce(const char *key, char *currentString)
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
 * @brief checks for an "OK" or "ERROR" and returns true or false respectively
 * @note in case none are found, the function also returns false
 *
 * @param currentString
 * @return true - an OK was found
 * @return false - either an ERROR was found or nothing was found
 */

/**
 * @brief checks for an "OK", "ERROR", or nothing and returns 1, 0, or -1 respectively
 *
 * @param currentString string to search in
 * @return int - 1, 0, or -1 for "OK", "ERROR", or "OK" respectively
 */
int ready(char *currentString)
{
    const char OK[] = "OK";
    const char ERROR[] = "ERROR";
    if (searchStringOnce(OK, currentString))
    {
        return 1;
    }
    else if (searchStringOnce(ERROR, currentString))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
/**
 * @brief appends a character to the string after checking for an "OK" or "ERROR"
 * @note case 0 probably means resend and display a warning/error, case 2 means resize or reset string, case 1 means continue to next command, case -1 should mean do nothing since a character is automatically added
 * @param currentString string to check
 * @param capacity capacity of string
 * @param input a character to append
 * @return int : 2, -2, 1, 0 for misc error, not found, found, error respectively
 */
int appendReady(char *currentString, const size_t capacity, char input)
{
    if (strlen(currentString) >= capacity)
    {
        return 2;
    }
    else if (ready(currentString) == -1)
    {
        strncat(currentString, &input, 1);
        return -1;
    }
    else if (ready(currentString) == 1)
    {
        return 1;
    }
    else if (ready(currentString) == 0)
    {
        return 0;
    }
    else
    {
        return 2;
    }
}
// +IPD,0,5:test
// TODO: use better function for splitting strings; ideally with a delimeter
int parseTCP(char *dataIn, char *onlyData)
{
    strcpy(onlyData, "");
    size_t link;
    char linkInChar[80] = "";
    char amountInChar[80] = "";
    char *walker = dataIn;
    // up to +IPD
    while (*walker != ',')
    {
        walker++;
    }
    walker++;
    // getting link_ID
    while (*walker != ',')
    {
        strncat(linkInChar, walker, 1);
        walker++;
    }
    walker++;
    link = atoi(linkInChar);
    // getting amount of bytes sent
    while (*walker != ':')
    {
        strncat(amountInChar, walker, 1);
        walker++;
    }
    // the rest of the string should be data
    // ensure that onlyData is large enough somehow
    strcat(onlyData, walker);
    return link;
}