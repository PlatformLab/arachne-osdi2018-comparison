#!/bin/bash

# This directory is used by Go to store raw data which we subsequently compute
# stats on.

mkdir data

# StdThread benchmarks
./ThreadCreationTest
./StdThreadCVWakeup

# Golang benchmarks
./GoThreadCreate && ./ExtractStats data/Go\ Thread\ Creation
./GoThreadCV && ./ExtractStats data/Go\ Condition\ Variable

# uThreads benchmarks

