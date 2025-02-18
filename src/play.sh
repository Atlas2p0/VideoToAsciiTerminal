#!/bin/bash
cwd=$(pwd)
directory="$cwd/../processed-frames/ascii-art"
fps=85
interval=$(echo "scale=6; 1 / $fps" | bc)
numberOfFrames=$(find "$directory" -type f | wc -l)

display_frame() {
    local frame_file="$1"
    tput cup 0 0
    cat "$frame_file"

}

frameNumber=1


for i in $(seq 1 $numberOfFrames); do
    frame_file="$directory/ascii_frame_$frameNumber.txt"
    display_frame "$frame_file"
        start_time=$(date +%s%N)
    # Sleep for the calculated interval
    sleep "$interval"
    ((frameNumber++))
done

sleep "2"
clear
    