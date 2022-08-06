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
bool ready(char *currentString)
{
    const char OK[] = "OK";
    const char ERROR[] = "ERROR";
    if (searchStringOnce(OK, currentString))
    {
        return true;
    }
    else if (searchStringOnce(ERROR, currentString))
    {
        return false;
    }
    else
    {
        return false;
    }
}