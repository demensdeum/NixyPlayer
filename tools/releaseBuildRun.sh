#!/bin/bash
clear
./tools/private/build.sh
if [ $? -eq 0 ]; then
    ./build/NixyPlayer ./samples/helloWorld/main.js
fi