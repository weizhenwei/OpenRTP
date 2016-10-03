#!/bin/sh

# exit when command return value != 0.
set -e
# print verbose debug message
# set -x

# avoid ctrl + c impact on this script;
os=`uname`
if [ x$os = x"Linux" ]; then
    echo "Running on Linux platform"
    trap '' INT
elif [ x$os = x"Darwin" ]; then
    echo "Running on Mac platform"
    # trap '' INT
fi

# echo "Checking code format"
# ./tools/format_check.sh;

echo "Root workspace directory: `pwd`"

cd build
echo "Working on directory: `pwd`"
./buildcmdline.sh Debug

cd cmdline
echo "Working on directory: `pwd`"
make VERBOSE=1
./src/OpenRTP
make test

cd ../..
echo "Returning directory: `pwd`"

