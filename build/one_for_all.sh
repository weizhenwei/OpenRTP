#!/bin/bash

# exit when command return value != 0.
set -e

# print verbose debug message
# set -x

echo "Building Makefile with cmake"
./buildcmdline.sh Debug

echo "Compiling source code"
cd cmdline
make

echo "Running OpenRTP and unittest"
./src/example/OpenRTP
./unittest/foo/runFooTests
./unittest/openrtp/runOpenRTPTests

echo "Done!"
cd ..

exit 0

