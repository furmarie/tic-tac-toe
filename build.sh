#!/bin/sh

build_web=0

while getopts "w" opt
do
    case $opt in
        (w) build_web=1;
    esac
done

echo "Starting build"

mkdir -p build/desktop
pushd build/desktop
    set -xv
    cmake ../..
    cmake --build .
    set +xv
popd

if [[ $build_web -eq 0 ]]; then 
    echo -e "\nTo build for web run with \"-w\""
    echo -e "\t./build.sh -w"
    exit 0; 
fi

echo "Building for web"
mkdir -p build/web
pushd build/web
    set -xv
    emcmake cmake ../..
    cmake --build .
    set +xv
popd

cp build/ttt.{js,wasm} app/
