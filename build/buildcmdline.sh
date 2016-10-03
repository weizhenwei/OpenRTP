#!/bin/bash

# set -x

if [ "x$#" != "x1" ] ; then
    echo "Usage ./buildcmdline.sh <Debug|Release>"
    exit 1
fi
if [ "x$1" != "xDebug" ] && [ "x$1" != "xRelease" ] ; then
    echo "Usage ./buildcmdline.sh <Debug|Release>"
    exit 1
fi

BUILD_TYPE=$1

cd cmdline
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -G "Unix Makefiles" ../..
# cd ..

exit 0

