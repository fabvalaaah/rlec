# rlec

Bitmap (bmp) file Run-Length Encoding (RLE) compression and decompression command line program, written in ANSI C for Linux.

RLE is a bitmap native lossless compression algorithm. As a consequence, RLE bitmap files resulting from this program are supported by most of image viewers and if so, can be opened as is.

This implementation deals directly with bitmap images without the usage of any third-party library.

## Installation

Compilation (using `gcc`) ⇨ `./COMPILE.sh`<br/>
Usage example ⇨ `./EXAMPLE.sh`<br/>
Cleaning (removes binary and test generated files) ⇨ `./CLEAN.sh`

## Usage

NAME<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;rlec - compress/decompress a bitmap file using RLE algorithm

SYNOPSIS<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`rlec {-c|-d} [-n=<OUTPUT_FILE_PATH>] <INPUT_FILE_PATH>`

OPTIONS<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Compress/decompress the file INPUT_FILE_PATH.<br/><br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-c&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;compression mode<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-d&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;decompression mode<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-n=<OUTPUT_FILE_PATH><br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;path to output file (default is the same directory as the input file)

File resulting from compression operation is automatically created with the suffix `_rle` added to the name of the input file.<br/>
File resulting from decompression operation is automatically created with the suffix `_raw` added to the name of the input file.

## Disclaimer

I am not responsible in any way of any consequence of the usage of this piece of software. You are warned, use it at your own risks.
