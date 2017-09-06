#!/bin/bash

# for i in {1..15}; do ./GoThreadCreationScalability $i 5; done
for i in {1..15}; do ./ThreadCreationScalability.sh $i 5; done
