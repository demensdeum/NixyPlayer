#!/bin/bash
clear
./tools/private/build.sh
if [ $? -ne 0 ]; then
    echo "Nixi Player build failed"
    exit 1
fi
cd extensions/base/
./tools/build.sh

if [ $? -ne 0 ]; then
    echo "Nixi Player base extension build failed"
    exit 1
fi

cd ../..
cp extensions/base/build/libNixyPlayerBaseExtension.so build/libNixyPlayerBaseExtension.so
cp resources/extensions.nixyp build

cd build
./NixyPlayer ../samples/helloWorld/main.js