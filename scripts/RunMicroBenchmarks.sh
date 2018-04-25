#!/bin/bash

# This directory is used by Go to store raw data which we subsequently compute
# stats on.

mkdir -p data

# StdThread benchmarks
output=$(bin/ThreadCreationTest)
output+=$'\n'$(bin/StdThreadCVWakeup | tail -n1)

# Golang benchmarks
output+=$'\n'$(bin/GoThreadCreate && bin/ExtractStats data/Go\ Thread\ Creation "Go Thread Creation" | tail -n1)
output+=$'\n'$(bin/GoThreadCV && bin/ExtractStats data/Go\ Condition\ Variable "Go Condition Variable" | tail -n1)

echo "$output" | scripts/column.py -s,
