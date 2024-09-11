#!/bin/bash

directory="/home/dawnwalker/playground/ascii-encoding/processed-frames/ascii-art"
fps=60
interval=$(echo "scale=2; 1 / $fps" | bc)

display_frame() {
    local frame_file="$1"
    clear

    cat "$frame_file"

}

frameNumber=1

while true; do

    for i in {0..13144..1}; do
        frame_file="/home/dawnwalker/playground/ascii-encoding/processed-frames/ascii-art/ascii_frame_$frameNumber.txt"
        display_frame "$frame_file"
        sleep "$interval"
        ((frameNumber++))
    done
done
    