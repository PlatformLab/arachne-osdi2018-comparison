#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: ./ThreadCreationScalability.sh <NumCores> <Duration_Seconds>"
    exit
fi

if [ "$1" -le 7 ]; then
    sudo env "PATH=$PATH" TakeCores.sh $(seq 1 $1 | paste -s -d, /dev/stdin) 0 $$
else
    i=$1
    endIndex=$((i - 8 + 16))
    cores=$(echo -n 1-7,; seq 16 $endIndex | paste -s -d, /dev/stdin)
    sudo env "PATH=$PATH" TakeCores.sh $cores  0 $$
fi
    ./ThreadCreationScalability $1 $2
    sudo env "PATH=$PATH" ReleaseCores.sh > /dev/null

# More than 7, we have discontiguous range of cores to stay on one socket.

