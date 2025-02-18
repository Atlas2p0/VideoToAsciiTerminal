#!/bin/bash

set -e

cwd=$(pwd)

./preprocessing.sh $1

CMAKE_LISTS_DIR="$cwd/../"
BUILD_DIR="$cwd/../build/"

cmake -S "$CMAKE_LISTS_DIR" -B "$BUILD_DIR"

make --directory="$BUILD_DIR"

cd "$BUILD_DIR"

./my_project

cd "$cwd"

clear

./play.sh
