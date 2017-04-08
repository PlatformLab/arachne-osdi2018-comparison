#!/bin/bash

for i in {1..7}; do ./GoThreadCreationScalability $i 5; done
for i in {1..7}; do ./ThreadCreationScalability.sh $i 5; done
