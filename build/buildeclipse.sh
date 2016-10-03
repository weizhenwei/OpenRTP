#!/bin/bash

# set -x

if [ "x$#" != "x1" ] ; then
    echo "Usage ./buildeclipse.sh <Debug|Release>"
    exit 1
fi
if [ "x$1" != "xDebug" ] && [ "x$1" != "xRelease" ] ; then
    echo "Usage ./buildeclipse.sh <Debug|Release>"
    exit 1
fi

BUILD_TYPE=$1

cd eclipse
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -G "Eclipse CDT4 - Unix Makefiles" ../..
cd ..

exit 0

