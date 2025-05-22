#!/bin/bash

set -e 

echo "ARM64 서버 빌드 중..."
rm -rf build-arm64
mkdir build-arm64
cd build-arm64
cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-arm64.cmake
make pop_server
cd ..

echo "클라이언트 빌드 중..."
rm -rf build-x86
mkdir build-x86
cd build-x86
cmake ..
make client
cd ..

echo "모든 빌드 완료"
echo "서버 실행파일: build-arm64/server/pop_server"
echo "클라이언트 실행파일: build-x86/client/client"

