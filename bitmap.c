/*
 * MIT License
 * 
 * Copyright (c) 2018 Fabvalaaah - fabvalaaah@laposte.net
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * DONATION:
 * As I share these sources for commercial use too, maybe you could consider
 * sending me a reward (even a tiny one) to my Ethereum wallet at the address
 * 0x1fEaa1E88203cc13ffE9BAe434385350bBf10868
 * If so, I would be forever grateful to you and motivated to keep up the good
 * work for sure :oD Thanks in advance !
 * 
 * FEEDBACK:
 * You like my work? It helps you? You plan to use/reuse/transform it? You have
 * suggestions or questions about it? Just want to say "hi"? Let me know your
 * feedbacks by mail to the address fabvalaaah@laposte.net
 * 
 * DISCLAIMER:
 * I am not responsible in any way of any consequence of the usage
 * of this piece of software. You are warned, use it at your own risks.
 */

/* 
 * File:   bitmap.c
 * Author: Fabvalaaah
 *
 * 02/26/2018
 */

#include "bitmap.h"

const uint8_t lineFeed = 0x00;
const uint8_t imageEnd = 0x01;

void printHeader(_bitmap* image) {
    fprintf(stdout, "\tMagic number: %c%c\n", image->magicNumber[0],
            image->magicNumber[1]);
    fprintf(stdout, "\tSize: %u\n", image->size);
    fprintf(stdout, "\tReserved: %u %u %u %u\n", image->reserved[0],
            image->reserved[1], image->reserved[2], image->reserved[3]);
    fprintf(stdout, "\tStart offset: %u\n", image->startOffset);
    fprintf(stdout, "\tHeader size: %u\n", image->headerSize);
    fprintf(stdout, "\tWidth: %u\n", image->width);
    fprintf(stdout, "\tHeight: %u\n", image->height);
    fprintf(stdout, "\tPlanes: %u\n", image->planes);
    fprintf(stdout, "\tDepth: %u\n", image->depth);
    fprintf(stdout, "\tCompression: %u\n", image->compression);
    fprintf(stdout, "\tImage size: %u\n", image->imageSize);
    fprintf(stdout, "\tX pixels per meters: %u\n", image->xPPM);
    fprintf(stdout, "\tY pixels per meters: %u\n", image->yPPM);
    fprintf(stdout, "\tNb of colors used: %u\n", image->nUsedColors);
    fprintf(stdout, "\tNb of important colors: %u\n", image->nImportantColors);
}

int RLECompression(FILE* ptrIn, FILE* ptrOut) {
    uint32_t i, total = 0, lecture = 0;
    bool ok = true;
    _bitmap* image = NULL;
    uint8_t* pallet = NULL;
    uint8_t* data = NULL;
    uint8_t occ = 0, cur, cur2;
    bool readRef = true;

    if (!ptrIn || !ptrOut) {
        return EXIT_FAILURE;
    }

    /* Reading the file */
    image = malloc(sizeof (_bitmap));
    if (!image) {
        fprintf(stderr, "Error while allocating memory\n");
        ok = false;
        goto cleaning;
    }
    memset(image, 0x00, sizeof (_bitmap));
    lecture = fread(image, 1, sizeof (_bitmap), ptrIn);
    if (lecture != sizeof (_bitmap)) {
        fprintf(stderr, "Error while reading data\n");
        ok = false;
        goto cleaning;
    }
    /* ------- */

    /* Controlling the magic number and the header size */
    if (image->magicNumber[0] != 'B' || image->magicNumber[1] != 'M'
            || image->headerSize != 40 || image->depth != 8
            || image->compression) {
        fprintf(stderr, "Error: Incompatible file type\n");
        ok = false;
        goto cleaning;
    }
    /* ------- */

    /* Controlling the start offset */
    if (image->startOffset < sizeof (_bitmap)) {
        fprintf(stderr, "Error: Wrong start offset\n");
        ok = false;
        goto cleaning;
    }
    /* ------- */

    printHeader(image);

    /* Skipping data from the beginning to the start offset */
    pallet = malloc(image->startOffset - sizeof (_bitmap));
    if (!pallet) {
        fprintf(stderr, "Error while allocating memory\n");
        ok = false;
        goto cleaning;
    }
    for (i = 0; i < (image->startOffset - sizeof (_bitmap))
            / FILE_BUFFER_SIZE; i++) {
        lecture = fread(pallet + i * FILE_BUFFER_SIZE, 1, FILE_BUFFER_SIZE,
                ptrIn);
        if (lecture != FILE_BUFFER_SIZE) {
            fprintf(stderr, "Error while reading input data\n");
            ok = false;
            goto cleaning;
        }
    }
    lecture = fread(pallet + i * FILE_BUFFER_SIZE, 1, (image->startOffset
            - sizeof (_bitmap)) % FILE_BUFFER_SIZE, ptrIn);
    if (lecture != (image->startOffset - sizeof (_bitmap)) % FILE_BUFFER_SIZE) {
        fprintf(stderr, "Error while reading input data\n");
        ok = false;
        goto cleaning;
    }
    /* ------- */

    /* RLE compression */
    data = malloc(2 * image->imageSize);
    if (!data) {
        fprintf(stderr, "Error while allocating memory\n");
        ok = false;
        goto cleaning;
    }
    for (i = 0; i < image->imageSize; i++) {
        /* End of line */
        if (i && !(i % image->width)) {
            if (occ) {
                data[total] = occ;
                total++;
                data[total] = cur;
                total++;
            }
            data[total] = 0x00;
            total++;
            data[total] = lineFeed;
            total++;
            occ = 0;
            readRef = true;
        }
        /* ------- */
        /* Max occurences */
        if (255 == occ) {
            data[total] = occ;
            total++;
            data[total] = cur;
            total++;
            occ = 0;
            readRef = true;
        }
        /* ------- */
        if (readRef) {
            lecture = fread(&cur, 1, 1, ptrIn);
            if (lecture != 1) {
                fprintf(stderr, "Error while reading input data\n");
                ok = false;
                goto cleaning;
            }
            occ++;
            readRef = false;
        } else {
            lecture = fread(&cur2, 1, 1, ptrIn);
            if (lecture != 1) {
                fprintf(stderr, "Error while reading input data\n");
                ok = false;
                goto cleaning;
            }
            if (cur == cur2) {
                occ++;
            } else {
                data[total] = occ;
                total++;
                data[total] = cur;
                total++;
                occ = 1;
                cur = cur2;
            }
        }
    }
    /* ------- */

    /* End of the last line */
    if (occ) {
        data[total] = occ;
        total++;
        data[total] = cur;
        total++;
    }
    /* ------- */
    /* End of the pic */
    data[total] = 0x00;
    total++;
    data[total] = imageEnd;
    total++;
    /* ------- */

    fprintf(stdout, "Compression ratio = %f%%\n", 100.0 - (image->startOffset
            + total) * 100.0 / image->size);

    /* Updating header informations */
    image->imageSize = total;
    image->size = image->startOffset + image->imageSize;
    image->compression = 0x0001;
    /* ------- */

    /* Writing the file */
    fwrite(image, sizeof (_bitmap), 1, ptrOut);
    fwrite(pallet, image->startOffset - sizeof (_bitmap), 1, ptrOut);
    fwrite(data, total, 1, ptrOut);
    /* ------- */

cleaning:
    if (image) {
        free(image);
        image = NULL;
    }

    if (pallet) {
        free(pallet);
        pallet = NULL;
    }

    if (data) {
        free(data);
        data = NULL;
    }

    if (!ok) {
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }
}

int RLEDecompression(FILE* ptrIn, FILE* ptrOut) {
    bool ok = true;
    _bitmap* image = NULL;
    uint32_t i, j, total = 0, lecture = 0;
    uint8_t* pallet = NULL;
    uint8_t* data = NULL;
    uint8_t couples[2];

    if (!ptrIn || !ptrOut) {
        return EXIT_FAILURE;
    }

    /* Reading the file */
    image = malloc(sizeof (_bitmap));
    if (!image) {
        fprintf(stderr, "Error while allocating memory\n");
        ok = false;
        goto cleaning;
    }
    memset(image, 0x00, sizeof (_bitmap));
    lecture = fread(image, 1, sizeof (_bitmap), ptrIn);
    if (lecture != sizeof (_bitmap)) {
        fprintf(stderr, "Error while reading data\n");
        ok = false;
        goto cleaning;
    }
    /* ------- */

    /* Controlling the magic number and the header size */
    if (image->magicNumber[0] != 'B' || image->magicNumber[1] != 'M'
            || image->headerSize != 40 || image->depth != 8
            || image->compression != 1) {
        fprintf(stderr, "Error: Incompatible file type\n");
        ok = false;
        goto cleaning;
    }
    /* ------- */

    /* Controlling the start offset */
    if (image->startOffset < sizeof (_bitmap)) {
        fprintf(stderr, "Error: Wrong start offset\n");
        ok = false;
        goto cleaning;
    }
    /* ------- */

    printHeader(image);

    /* Skipping data from the beginning to the start offset */
    pallet = malloc(image->startOffset - sizeof (_bitmap));
    if (!pallet) {
        fprintf(stderr, "Error while allocating memory\n");
        ok = false;
        goto cleaning;
    }
    for (i = 0; i < (image->startOffset - sizeof (_bitmap))
            / FILE_BUFFER_SIZE; i++) {
        lecture = fread(pallet + i * FILE_BUFFER_SIZE, 1, FILE_BUFFER_SIZE,
                ptrIn);
        if (lecture != FILE_BUFFER_SIZE) {
            fprintf(stderr, "Error while reading input data\n");
            ok = false;
            goto cleaning;
        }
    }
    lecture = fread(pallet + i * FILE_BUFFER_SIZE, 1, (image->startOffset
            - sizeof (_bitmap)) % FILE_BUFFER_SIZE, ptrIn);
    if (lecture != (image->startOffset - sizeof (_bitmap)) % FILE_BUFFER_SIZE) {
        fprintf(stderr, "Error while reading input data\n");
        ok = false;
        goto cleaning;
    }
    /* ------- */

    /* RLE decompression */
    data = malloc(image->width * image->height);
    if (!data) {
        fprintf(stderr, "Error while allocating memory\n");
        ok = false;
        goto cleaning;
    }
    for (i = 0; i < image->imageSize / 2; i++) {
        lecture = fread(&couples, 2, 1, ptrIn);
        if (lecture != 1) {
            fprintf(stderr, "Error while reading input data\n");
            ok = false;
            goto cleaning;
        }
        if (couples[0]) {
            for (j = 0; j < couples[0]; j++) {
                data[total] = couples[1];
                total++;
            }
        }
    }
    /* ------- */

    /* Updating header informations */
    image->imageSize = total;
    image->size = image->startOffset + image->imageSize;
    image->compression = 0x0000;
    /* ------- */

    /* Writing the file */
    fwrite(image, sizeof (_bitmap), 1, ptrOut);
    fwrite(pallet, image->startOffset - sizeof (_bitmap), 1, ptrOut);
    fwrite(data, total, 1, ptrOut);
    /* ------- */

cleaning:
    if (image) {
        free(image);
        image = NULL;
    }

    if (pallet) {
        free(pallet);
        pallet = NULL;
    }

    if (data) {
        free(data);
        data = NULL;
    }

    if (!ok) {
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }
}