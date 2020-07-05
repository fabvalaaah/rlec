#!/bin/sh

TEST_SAMPLES_DIR="./samples"
ORIGINAL_FILE="grey.bmp"
RLE_FILE="grey_rle.bmp"

./COMPILE.sh
./rlec -c -n="$RLE_FILE" "$TEST_SAMPLES_DIR/$ORIGINAL_FILE"
echo
./rlec -d -n="$ORIGINAL_FILE" "$RLE_FILE"