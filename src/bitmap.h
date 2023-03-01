#ifndef BITMAP_H
#define BITMAP_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "common.h"

typedef struct __attribute__((__packed__)) _bitmap
{
    uint8_t magicNumber[2];
    uint32_t size;
    uint8_t reserved[4];
    uint32_t startOffset;
    uint32_t headerSize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t depth;
    uint32_t compression;
    uint32_t imageSize;
    uint32_t xPPM;
    uint32_t yPPM;
    uint32_t nUsedColors;
    uint32_t nImportantColors;
} _bitmap;

void printHeader(_bitmap *image);
int RLECompression(FILE *ptrIn, FILE *ptrOut);
int RLEDecompression(FILE *ptrIn, FILE *ptrOut);

#endif /* BITMAP_H */
