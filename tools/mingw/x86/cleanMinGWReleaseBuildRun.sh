rm -rf ./build
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DMINGW32=1 ../src/ && make
cd ..

cd extensions/base/
rm -rf ./build
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DMINGW32=1 ../src/ && make

cd ../../../
rm -rf release
mkdir -p release
cp samples/helloWorld/main.js release
cp build/NixyPlayer.exe release
cp extensions/base/build/NixyPlayerBaseExtension.dll release
cp resources/Windows_extensions.nixyp release/extensions.nixyp
cp /usr/i686-w64-mingw32/bin/libwinpthread-1.dll release
cp /usr/i686-w64-mingw32/bin/libgcc_s_dw2-1.dll release
cp /usr/i686-w64-mingw32/bin/libstdc++-6.dll release

cd release

wine NixyPlayer.exe main.js --verbose

if [ $? -eq 0 ]; then
    zip -r NixyPlayer-1.0.0.zip .
fi