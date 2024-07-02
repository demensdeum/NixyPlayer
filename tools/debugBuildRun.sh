#!/bin/bash
clear
./tools/private/build.sh --debug-build
if [ $? -eq 0 ]; then
    ./build/NixyPlayer ./samples/helloWorld/main.js --verbose
fi