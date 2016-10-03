#!/bin/bash

# set -x

if [ "x$#" != "x1" ] ; then
    echo "Usage ./buildxcode.sh <Debug|Release>"
    exit 1
fi
if [ "x$1" != "xDebug" ] && [ "x$1" != "xRelease" ] ; then
    echo "Usage ./buildxcode.sh <Debug|Release>"
    exit 1
fi

BUILD_TYPE=$1

cd xcode
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -G "Xcode" ../..
cd ..

exit 0

