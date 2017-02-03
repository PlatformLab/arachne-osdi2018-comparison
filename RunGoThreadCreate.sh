#!/bin/bash

for i in 2 3 4; do
    echo "GOMAXPROCS = $i"
    GOMAXPROCS=$i ./GoThreadCreate > GoThreadCreate.log
    ttsum.py GoThreadCreate.log
    echo
done
