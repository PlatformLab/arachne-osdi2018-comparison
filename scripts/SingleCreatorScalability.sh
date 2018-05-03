#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: ./SingleCreatorScalability.sh <NumCores> <Duration_Seconds>"
    exit
fi

sudo env "PATH=$PATH" TakeCores.sh $(seq 1 $1 | paste -s -d, /dev/stdin) 0 $$
bin/SingleCreatorScalability $1 $2
sudo env "PATH=$PATH" ReleaseCores.sh > /dev/null
