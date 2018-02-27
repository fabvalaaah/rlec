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
 * File:   main.c
 * Author: Fabvalaaah
 *
 * 02/26/2018
 */

#include "main.h"
#include "bitmap.h"

void printUsage() {
    fprintf(stderr, "Usage: rlec -c || -d [-n=OUTPUT_BMP_FILE_PATH] INPUT_BMP_FILE_PATH\n");
    fprintf(stderr, "options:\n");
    fprintf(stderr, "\t-c: File compression\n");
    fprintf(stderr, "\t-d: File decompression\n");
    fprintf(stderr, "\t-n=OUTPUT_BMP_FILE_PATH: Output file path\n");
}

int compressBitmap(const char* inputPath, const char* out) {
    FILE* ptrIn = NULL;
    FILE* ptrOut = NULL;
    char* outputPath = NULL;
    bool ok = true;

    if (!inputPath) {
        return EXIT_FAILURE;
    }

    /* File opening */
    ptrIn = openFile(inputPath, "rb");
    if (!ptrIn) {
        fprintf(stderr, "Error while opening the input file\n");
        return EXIT_FAILURE;
    }
    if (!out) {
        outputPath = malloc(strlen(inputPath) + strlen(cTerm) + 1);
        if (!outputPath) {
            ok = false;
            goto cleaning;
        }
        memset(outputPath, '\0', strlen(inputPath) + strlen(cTerm) + 1);
        sprintf(outputPath, "%s%s", inputPath, cTerm);
        ptrOut = openFile(outputPath, "wb");
        fprintf(stdout, "Compressing the file \"%s\" to \"%s\"\n", inputPath,
                outputPath);
    } else {
        if (!strcmp(inputPath, out)) {
            fprintf(stderr, "Input and output files must be different\n");
            ok = false;
            goto cleaning;
        }
        ptrOut = openFile(out, "wb");
        fprintf(stdout, "Compressing the file \"%s\" to \"%s\"\n", inputPath,
                out);
    }
    if (!ptrOut) {
        fprintf(stderr, "Error while opening the output file\n");
        ok = false;
        goto cleaning;
    }
    /* ------- */

    ok = !RLECompression(ptrIn, ptrOut);

cleaning:
    if (!ok) {
        if (out && strcmp(inputPath, out)) {
            remove(out);
        }
        if (outputPath) {
            remove(outputPath);
        }
    }

    /* Closing the files */
    closeFile(&ptrIn);
    closeFile(&ptrOut);
    /* ------- */

    if (outputPath) {
        free(outputPath);
        outputPath = NULL;
    }

    if (!ok) {
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }
}

int decompressBitmap(const char* inputPath, const char* out) {
    FILE* ptrIn = NULL;
    FILE* ptrOut = NULL;
    char* outputPath = NULL;
    bool ok = true;

    if (!inputPath) {
        return EXIT_FAILURE;
    }

    /* File opening */
    ptrIn = openFile(inputPath, "rb");
    if (!ptrIn) {
        fprintf(stderr, "Error while opening the input file\n");
        return EXIT_FAILURE;
    }
    if (!out) {
        outputPath = malloc(strlen(inputPath) + strlen(dTerm) + 1);
        if (!outputPath) {
            ok = false;
            goto cleaning;
        }
        memset(outputPath, '\0', strlen(inputPath) + strlen(dTerm) + 1);
        sprintf(outputPath, "%s%s", inputPath, dTerm);
        ptrOut = openFile(outputPath, "wb");
        fprintf(stdout, "Decompressing the file \"%s\" to \"%s\"\n", inputPath,
                outputPath);
    } else {
        if (!strcmp(inputPath, out)) {
            fprintf(stderr, "Input and output files must be different\n");
            ok = false;
            goto cleaning;
        }
        ptrOut = openFile(out, "wb");
        fprintf(stdout, "Decompressing the file \"%s\" to \"%s\"\n", inputPath,
                out);
    }
    if (!ptrOut) {
        fprintf(stderr, "Error while opening the output file\n");
        ok = false;
        goto cleaning;
    }
    /* ------- */

    ok = !RLEDecompression(ptrIn, ptrOut);

cleaning:
    if (!ok) {
        if (out && strcmp(inputPath, out)) {
            remove(out);
        }
        if (outputPath) {
            remove(outputPath);
        }
    }

    /* Closing the files */
    closeFile(&ptrIn);
    closeFile(&ptrOut);
    /* ------- */

    if (outputPath) {
        free(outputPath);
        outputPath = NULL;
    }

    if (!ok) {
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }
}

int manageCommand(int argc, const char* argv[]) {
    int i, ret = EXIT_FAILURE;
    char compression = 0;
    char* name = NULL;

    if (argc < 3 || argc > 4) {
        printUsage();
        return EXIT_FAILURE;
    }

    for (i = 1; i <= argc - 2; i++) {
        if (!strcmp(argv[i], "-c")) {
            compression = 1;
        } else if (!strcmp(argv[i], "-d")) {
            compression = 2;
        } else if (!strncmp(argv[i], "-n=", 3)) {
            name = malloc(strlen(argv[i]) - 2);
            if (!name) {
                fprintf(stderr, "Error while allocating memory\n");
                return EXIT_FAILURE;
            }
            memset(name, '\0', strlen(argv[i]) - 2);
            strncpy(name, argv[i] + 3, strlen(argv[i]) - 3);
        }
    }

    if (!compression) {
        printUsage();
        if (name) {
            free(name);
            name = NULL;
        }
        return EXIT_FAILURE;
    } else if (1 == compression) {
        ret = compressBitmap(argv[argc - 1], name);
    } else if (2 == compression) {
        ret = decompressBitmap(argv[argc - 1], name);
    }

    if (EXIT_FAILURE == ret) {
        printUsage();
    }
    if (name) {
        free(name);
        name = NULL;
    }
    return ret;
}

int main(int argc, const char* argv[]) {
    return manageCommand(argc, argv);
}