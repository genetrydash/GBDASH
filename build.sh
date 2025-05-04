#!/bin/bash

set -e

mkdir -p build

for file in src/*.c; do
  base=$(basename "$file" .c)
  echo "Compiling $file -> build/$base.o"
  lcc -Iinclude -c -o "build/$base.o" "$file"
done

OBJ_FILES=$(find build -name "*.o")
lcc -Iinclude -Wl-lhugedriver/gbdk/hUGEDriver.lib -o build/game.gb $OBJ_FILES
