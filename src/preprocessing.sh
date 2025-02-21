#!/bin/bash

set -e

echo "Checking for required packages.."

install_if_missing() {
    local pkg=$1
    if dpkg -s "$pkg" &> /dev/null; then
        echo "pkg is already installed."
    else
        echo "$pkg is not installed. Installing...."
        sudo apt-get install -y "$pkg"
    fi
}

install_if_missing "libopencv-dev"
install_if_missing "ffmpeg"

cwd=$(pwd)
FFMPEG_FRAMES_DIR="$cwd/../ffmpeg-framedata"

if [ ! -d "$FFMPEG_FRAMES_DIR" ]; then
    echo "Directory $FFMPEG_FRAMES_DIR doesn't exist. Creating..."
    mkdir -p "$FFMPEG_FRAMES_DIR"
else
    echo "Directory $FFMPEG_FRAMES_DIR exists"
fi

PROCESSED_FRAMES_DIR="$cwd/../processed-frames"

ASCII_FRAMES_DIR="$cwd/../processed-frames/ascii-art"

if [ ! -d "$PROCESSED_FRAMES_DIR" ]; then
    echo "Directory $UNPROCESSEED_FRAMES_DIR doesn't exist. Creating..."
    mkdir -p "$PROCESSED_FRAMES_DIR"
else
    echo "Directory $PROCESSED_FRAMES_DIR exists"
fi

echo "Cleaning ffmpeg frames dir..."

rm -rf "$PROCESSED_FRAMES_DIR/"*

if [ ! -d "$ASCII_FRAMES_DIR" ]; then
    echo "Directory $ASCII_FRAMES_DIR doesn't exist. Creating..."
    mkdir -p "$ASCII_FRAMES_DIR"
else
    echo "Directory $ASCII_FRAMES_DIR exists"
fi




echo "Cleaning ffmpeg frames dir..."

rm -rf "$FFMPEG_FRAMES_DIR/"*


echo "Transforming input video to greyscale frames...."

ffmpeg -threads 4 -i $1 -vf "scale=1400:1050,unsharp=5:5:1.0:5:5:0.0,eq=contrast=1.3:brightness=0.005,fps=60,format=gray" $FFMPEG_FRAMES_DIR/frame_%d.png

