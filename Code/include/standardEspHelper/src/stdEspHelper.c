#include "stdEspHelper.h"
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
