# rlec
C RLE bitmap (bmp) file compression and decompression.

This is a C NetBeans project, compiled with GCC 5.4.0 and tested with Ubuntu
Linux 16.04.5 LTS x64. No leak detected using Valgrind.

This piece of software converts a non-RLE bitmap file into a RLE bitmap file.
More informations about the Run-Length Encoding (RLE) algorithm can be found
here: https://en.wikipedia.org/wiki/Run-length_encoding

This implementation deals directly with bitmap (bmp) images without the usage of
any third-party library. RLE bmp are natively supported by most of image viewers
and can be opened easily. RLE is a lossless compression algorithm that works
best with pictures that have a tight color palette.

## Usage
Compression: "./rlec -c [-n=<output bmp file path>] <input bmp file path>".

Decompression: "./rlec -d [-n=<output bmp file path>] <input bmp file path>".

DONATION:
As I share these sources for commercial use too, maybe you could consider
sending me a reward (even a tiny one) to my Ethereum wallet at the address
0x1fEaa1E88203cc13ffE9BAe434385350bBf10868
If so, I would be forever grateful to you and motivated to keep up the good work
for sure :oD Thanks in advance !

FEEDBACK:
You like my work? It helps you? You plan to use/reuse/transform it? You have
suggestions or questions about it? Just want to say "hi"? Let me know your
feedbacks by mail to the address fabvalaaah@laposte.net

DISCLAIMER:
I am not responsible in any way of any consequence of the usage of this piece of
software. You are warned, use it at your own risks.