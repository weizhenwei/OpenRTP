#!/bin/sh

gcc -g client.c reportblock.h -o aclient
gcc -g server.c reportblock.h -o aserver

