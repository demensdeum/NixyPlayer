#!/bin/bash
clear
./tools/private/build.sh
if [ $? -ne 0 ]; then
    echo "Nixy Player build failed"
    exit 1
fi
cd extensions/base/
./tools/build.sh

if [ $? -ne 0 ]; then
    echo "Nixy Player base extension build failed"
    exit 1
fi

cd ../..
pwd
cp extensions/base/build/libNixyPlayerBaseExtension.so build/libNixyPlayerBaseExtension.so
cp extensions/base/build/libNixyPlayerBaseExtension.dylib build/libNixyPlayerBaseExtension.dylib
cp resources/extensions.nixyp build

cd build
./NixyPlayer ../samples/helloWorld/main.js