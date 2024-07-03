rm -rf ./build
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DMINGW32=1 ../src/ && make
cd ..

cd extensions/base/
rm -rf ./build
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DMINGW32=1 ../src/ && make

cd ../../../
rm -rf release
mkdir -p release
cp samples/helloWorld/main.js release
cp build/NixyPlayer.exe release
cp extensions/base/build/NixyPlayerBaseExtension.dll release
cp resources/Windows_extensions.nixyp release/extensions.nixyp
cd release
wine NixyPlayer.exe main.js --verbose