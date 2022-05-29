#!/bin/sh

root="$(dirname "$(readlink -f "$0")")"

if [ "$1" = "linux" ]; then
    rm -rf "$root/build/linux"
    mkdir -p "$root/build/linux"
    cmake -S "$root" -B "$root/build/linux"
elif [ "$1" = "mingw" ]; then
    rm -rf "$root/build/mingw"
    mkdir -p "$root/build/mingw"
    cmake -S "$root" -B "$root/build/mingw" -DCMAKE_C_COMPILER=x86_64-w64-mingw32-cc
elif [ "$1" = "macos" ]; then
    rm -rf "$root/build/macos"
    mkdir -p "$root/build/macos"
    cmake -S "$root" -B "$root/build/macos"
elif [ "$1" = "android" ]; then
    rm -rf "$root/build/android"
    mkdir -p "$root/build/android"
    cmake -S "$root" -B "$root/build/android" -DCMAKE_TOOLCHAIN_FILE=~/Android/Sdk/ndk/24.0.8215888/build/cmake/android.toolchain.cmake -DANDROID_ABI=x86_64
elif [ "$1" = "ios" ]; then
    rm -rf "$root/build/ios"
    mkdir -p "$root/build/ios"
    cmake -S "$root" -B "$root/build/android" -GXcode -DCMAKE_SYSTEM_NAME=iOS "-DCMAKE_OSX_ARCHITECTURES=armv7;armv7s;arm64;i386;x86_64"
else
    echo "Unknown platform $1"
fi