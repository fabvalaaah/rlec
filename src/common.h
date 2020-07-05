#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

#define FILE_BUFFER_SIZE 1024

FILE *openFile(const char *filePath, const char *mode);
void closeFile(FILE **ptr);

#endif /* COMMON_H */