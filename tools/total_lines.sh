#!/bin/bash

headerline=`find ./src -name \*.h | xargs cat | wc -l`;
cline=`find ./src -name \*.c | xargs cat | wc -l`;
cppline=`find ./src -name \*.cpp | xargs cat | wc -l`;
objcline=`find ./src -name \*.m | xargs cat | wc -l`;
objcppline=`find ./src -name \*.mm | xargs cat | wc -l`;

totalline=`expr $headerline + $cline + $cppline \
           + $objcline + $objcppline`;
echo "total code written in project = $totalline";

