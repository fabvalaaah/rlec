/**
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
 */

/**
 * DISCLAIMER:
 * I am not responsible in any way of any consequence of the usage of this piece
 * of software. You are warned, use it at your own risks.
 */

#include "main.h"
#include "bitmap.h"

void printUsage()
{
    fprintf(stderr, "NAME\n");
    fprintf(stderr, "\trlec - compress/decompress a bitmap file using RLE algorithm\n\n");
    fprintf(stderr, "SYNOPSIS\n");
    fprintf(stderr, "\trlec {-c|-d} [-n=<OUTPUT_FILE_PATH>] <INPUT_FILE_PATH>\n\n");
    fprintf(stderr, "OPTIONS\n");
    fprintf(stderr, "\tCompress/decompress the file located in INPUT_FILE_PATH.\n\n");
    fprintf(stderr, "\t-c\tcompression mode\n\n");
    fprintf(stderr, "\t-d\tdecompression mode\n\n");
    fprintf(stderr, "\t-n=<OUTPUT_FILE_PATH>\n");
    fprintf(stderr, "\t\tpath to output file (default is the same directory as the input file)\n\n");
}

int compressBitmap(const char *inputPath, const char *out)
{
    FILE *ptrIn = NULL;
    FILE *ptrOut = NULL;
    char *outputPath = NULL;
    bool ok = true;

    if (!inputPath)
    {
        return EXIT_FAILURE;
    }

    /* File opening */
    ptrIn = openFile(inputPath, "rb");
    if (!ptrIn)
    {
        fprintf(stderr, "Error while opening the input file\n");
        return EXIT_FAILURE;
    }
    if (!out)
    {
        outputPath = malloc(strlen(inputPath) + strlen(cTerm) + 1);
        if (!outputPath)
        {
            ok = false;
            goto cleaning;
        }
        memset(outputPath, '\0', strlen(inputPath) + strlen(cTerm) + 1);
        sprintf(outputPath, "%s%s", inputPath, cTerm);
        ptrOut = openFile(outputPath, "wb");
        fprintf(stdout, "Compressing the file \"%s\" to \"%s\"\n", inputPath,
                outputPath);
    }
    else
    {
        if (!strcmp(inputPath, out))
        {
            fprintf(stderr, "Input and output files must be different\n");
            ok = false;
            goto cleaning;
        }
        ptrOut = openFile(out, "wb");
        fprintf(stdout, "Compressing the file \"%s\" to \"%s\"\n", inputPath,
                out);
    }
    if (!ptrOut)
    {
        fprintf(stderr, "Error while opening the output file\n");
        ok = false;
        goto cleaning;
    }
    /* ------- */

    ok = !RLECompression(ptrIn, ptrOut);

cleaning:
    if (!ok)
    {
        if (out && strcmp(inputPath, out))
        {
            remove(out);
        }
        if (outputPath)
        {
            remove(outputPath);
        }
    }

    /* Closing the files */
    closeFile(&ptrIn);
    closeFile(&ptrOut);
    /* ------- */

    if (outputPath)
    {
        free(outputPath);
        outputPath = NULL;
    }

    if (!ok)
    {
        return EXIT_FAILURE;
    }
    else
    {
        return EXIT_SUCCESS;
    }
}

int decompressBitmap(const char *inputPath, const char *out)
{
    FILE *ptrIn = NULL;
    FILE *ptrOut = NULL;
    char *outputPath = NULL;
    bool ok = true;

    if (!inputPath)
    {
        return EXIT_FAILURE;
    }

    /* File opening */
    ptrIn = openFile(inputPath, "rb");
    if (!ptrIn)
    {
        fprintf(stderr, "Error while opening the input file\n");
        return EXIT_FAILURE;
    }
    if (!out)
    {
        outputPath = malloc(strlen(inputPath) + strlen(dTerm) + 1);
        if (!outputPath)
        {
            ok = false;
            goto cleaning;
        }
        memset(outputPath, '\0', strlen(inputPath) + strlen(dTerm) + 1);
        sprintf(outputPath, "%s%s", inputPath, dTerm);
        ptrOut = openFile(outputPath, "wb");
        fprintf(stdout, "Decompressing the file \"%s\" to \"%s\"\n", inputPath,
                outputPath);
    }
    else
    {
        if (!strcmp(inputPath, out))
        {
            fprintf(stderr, "Input and output files must be different\n");
            ok = false;
            goto cleaning;
        }
        ptrOut = openFile(out, "wb");
        fprintf(stdout, "Decompressing the file \"%s\" to \"%s\"\n", inputPath,
                out);
    }
    if (!ptrOut)
    {
        fprintf(stderr, "Error while opening the output file\n");
        ok = false;
        goto cleaning;
    }
    /* ------- */

    ok = !RLEDecompression(ptrIn, ptrOut);

cleaning:
    if (!ok)
    {
        if (out && strcmp(inputPath, out))
        {
            remove(out);
        }
        if (outputPath)
        {
            remove(outputPath);
        }
    }

    /* Closing the files */
    closeFile(&ptrIn);
    closeFile(&ptrOut);
    /* ------- */

    if (outputPath)
    {
        free(outputPath);
        outputPath = NULL;
    }

    if (!ok)
    {
        return EXIT_FAILURE;
    }
    else
    {
        return EXIT_SUCCESS;
    }
}

int manageCommand(int argc, const char *argv[])
{
    int i, ret = EXIT_FAILURE;
    char compression = 0;
    char *name = NULL;

    if (argc < 3 || argc > 4)
    {
        printUsage();
        return EXIT_FAILURE;
    }

    for (i = 1; i <= argc - 2; i++)
    {
        if (!strcmp(argv[i], "-c"))
        {
            compression = 1;
        }
        else if (!strcmp(argv[i], "-d"))
        {
            compression = 2;
        }
        else if (!strncmp(argv[i], "-n=", 3))
        {
            name = malloc(strlen(argv[i]) - 2);
            if (!name)
            {
                fprintf(stderr, "Error while allocating memory\n");
                return EXIT_FAILURE;
            }
            memset(name, '\0', strlen(argv[i]) - 2);
            strncpy(name, argv[i] + 3, strlen(argv[i]) - 3);
        }
    }

    if (!compression)
    {
        printUsage();
        if (name)
        {
            free(name);
            name = NULL;
        }
        return EXIT_FAILURE;
    }
    else if (1 == compression)
    {
        ret = compressBitmap(argv[argc - 1], name);
    }
    else if (2 == compression)
    {
        ret = decompressBitmap(argv[argc - 1], name);
    }

    if (EXIT_FAILURE == ret)
    {
        printUsage();
    }
    if (name)
    {
        free(name);
        name = NULL;
    }
    return ret;
}

int main(int argc, const char *argv[])
{
    return manageCommand(argc, argv);
}