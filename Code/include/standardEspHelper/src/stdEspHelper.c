#include "stdEspHelper.h"
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