#!/bin/bash

for i in 1 2 3 4; do
echo "GOMAXPROCS = $i"
GOMAXPROCS=$i ./GoThreadCreate > GoThreadCreate.log
ttsum.py GoThreadCreate.log
done
