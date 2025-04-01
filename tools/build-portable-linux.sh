#!/bin/bash

#
# Copyright (C) 2025 https://github.com/diroverflow
#
# This is free software. You can redistribute it and/or
# modify it under the terms of the GNU General Public License
# version 3 as published by the Free Software Foundation.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY.
#

DIR=$(cd "$(dirname "$0")" && pwd)

PROJECT_ROOT=$(cd $DIR/.. && pwd)
PORTABLE_DIR_REL_PATH="build/portable"
PORTABLE_ARCHIVE_NAME="ai-at-once-portable"

rm -rf build/
mkdir build && cd build

cmake -Dai-at-once_BUILD_PORTABLE=ON ..
make

cd $PROJECT_ROOT

mkdir $PORTABLE_DIR_REL_PATH
cp build/ai-at-once $PORTABLE_DIR_REL_PATH
cp /lib/x86_64-linux-gnu/libwx_gtk3u_core-3.0.so.0 $PORTABLE_DIR_REL_PATH
cp /lib/x86_64-linux-gnu/libwx_baseu-3.0.so.0 $PORTABLE_DIR_REL_PATH

cd $PORTABLE_DIR_REL_PATH

tar -czf ../$PORTABLE_ARCHIVE_NAME.tar.gz *