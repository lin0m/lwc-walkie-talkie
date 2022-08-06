#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
bool searchStringOnce(const char *key, char *currentString);
int ready(char *currentString);
int appendReady(char* currentString, const size_t capacity, char input);
int parseTCP(char *dataIn, char *onlyData);