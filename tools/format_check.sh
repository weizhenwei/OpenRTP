#!/bin/sh

# ATTENTION: This script should be run at the root dir of OpenRTP project.

find ./unittest -name *.h -o -name *.cpp | xargs ./tools/cpplint.py

find ./src -name *.h -o -name *.cpp | xargs ./tools/cpplint.py

