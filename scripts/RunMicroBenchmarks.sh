#!/bin/bash

# This directory is used by Go to store raw data which we subsequently compute
# stats on.

mkdir -p data

# StdThread benchmarks
bin/ThreadCreationTest
bin/StdThreadCVWakeup

# Golang benchmarks
bin/GoThreadCreate && bin/ExtractStats data/Go\ Thread\ Creation
bin/GoThreadCV && bin/ExtractStats data/Go\ Condition\ Variable

# uThreads benchmarks

