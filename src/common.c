#include "common.h"

FILE *openFile(const char *filePath, const char *mode)
{
    FILE *ptr = NULL;

    ptr = fopen(filePath, mode);

    return ptr;
}

void closeFile(FILE **ptr)
{
    if (!ptr || !(*ptr))
    {
        return;
    }

    fclose(*ptr);
    *ptr = NULL;
}