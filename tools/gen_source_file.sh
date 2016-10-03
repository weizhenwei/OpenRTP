#!/bin/bash

# exit when command return value != 0.
set -e

# print verbose debug message
# set -x

G_FULLPATH=""

function print_usage() {
echo  "Usage ./gen_source_file.sh <-f fullfilename> [-h]"
echo  "      -f, full path of file "
echo  "      -h, show help info"
}

function parse_cmdline() {
# semicolon means that the option needs parameters;
while getopts "f:h" arg
do
    case  $arg  in
        f)
            G_FULLPATH=$OPTARG
            echo  "file name is $OPTARG "  # parameter is in $OPTARG
            ;;
        h)
            print_usage
            exit 0
            ;;
        ? )  # unknown argument
            echo  " unkonw argument "
            exit  1
            ;;
    esac
done
}

copyright='* Copyright   :
 * Copyright (c) 2016, weizhenwei
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the {organization} nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *'

function write_header_file() {
fullfilename=$1
date=`date "+%Y.%m.%d"`

# get filename
fname=$1
OLD_IFS=$IFS
IFS="/"
array=($fname)
IFS=$OLD_IFS
len=${#array[*]} 
filename=${array[$len-1]}
echo "short filename is $filename"

# get header macro
headermacro=$1
headermacro=${headermacro//\//_}  # replace '/' with '_';
headermacro=${headermacro//./_}   # replace '.' with '_';
headermacro=`tr '[a-z]' '[A-Z]' <<<"$headermacro"` # translate to upper case;
echo "headermacro is $headermacro"

echo "/*
 ============================================================================
 * Name        : $filename
 * Author      : weizhenwei, <weizhenwei1988@gmail.com>
 * Date        : $date
 *
 $copyright
 * Description : header file of $filename
 ============================================================================
 */

#ifndef $headermacro
#define $headermacro

namespace openrtp {


}  // namespace openrtp

#endif  // $headermacro
" >> $fullfilename
}

function write_source_file() {
fullfilename=$1
date=`date "+%Y.%m.%d"`

# get filename
fname=$1
OLD_IFS=$IFS
IFS="/"
array=($fname)
IFS=$OLD_IFS
len=${#array[*]} 
filename=${array[$len-1]}
echo "short filename is $filename"

# get header filename
fname=$filename
OLD_IFS=$IFS
IFS="."
array=($fname)
IFS=$OLD_IFS
len=${#array[*]} 
headername=${array[0]}".h"
echo "header filename is $headername"

echo "/*
 ============================================================================
 * Name        : $filename
 * Author      : weizhenwei, <weizhenwei1988@gmail.com>
 * Date        : $date
 *
 $copyright
 * Description : implementation file of $filename
 ============================================================================
 */

#include \"$headername\"

namespace openrtp {


}  // namespace openrtp
" >> $fullfilename
}


######################## MAIN PART ############################

# step 1, parse cmdline and check parameters
parse_cmdline "$@"
if [ x$G_FULLPATH = x ] 
then
    print_usage
    exit 0
fi

# step 2, clean fullpath and check;
cleanfilename=${G_FULLPATH#"./"}  # remove heading './'
# get suffix h/cpp
suffix=`echo $cleanfilename | cut -d "." -f2`
if [ "x$suffix" = "xh" ]; then
    echo "suffix is $suffix, header file"
elif [ "x$suffix" = "xcpp" ]; then
    echo "suffix is $suffix, source file"
elif [ "x$suffix" = "x" ]; then
    echo "file type is null"
    exit 1
else
    echo "Unsupport file type $suffix"
    exit 1
fi

# step 3, check file existence and touch new file;
echo "cleanfilename is $cleanfilename"
# remove already existed file
if [ -f $cleanfilename ]; then
    echo "file $cleanfilename exist, remove first"
    /bin/rm -rf $cleanfilename
fi
touch $cleanfilename


# step 4, write file content according file suffix;
if [ "x$suffix" = "xh" ]; then
    write_header_file $cleanfilename
else
    write_source_file $cleanfilename
fi

echo "touch and write code to file $cleanfilename"

