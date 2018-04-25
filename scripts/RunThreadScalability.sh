#!/bin/bash

# for i in {1..15}; do ./GoThreadCreationScalability $i 5; done

# Give CPU sets a rest in between runs
for i in {1..15}; do scripts/ThreadCreationScalability.sh $i 5 ; sleep 1 ; done
