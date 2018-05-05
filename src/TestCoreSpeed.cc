#include <stdlib.h>
#include <atomic>
#include "PerfUtils/Cycles.h"
#include "PerfUtils/TimeTrace.h"


using PerfUtils::Cycles;
using PerfUtils::TimeTrace;

std::atomic<uint64_t> globalCount;

// Both of these times are in cycles
std::atomic<uint64_t> stopTime;
std::atomic<uint64_t> startTime;

// This time is in seconds
std::atomic<double> duration;

/**
 * Pass in fixed number of cores, duration to run for in seconds.
 */
int main(int argc, const char** argv) {
    startTime = Cycles::rdtsc();
    uint64_t durationInCycles = Cycles::fromSeconds(5);
    stopTime = Cycles::rdtsc() + durationInCycles;
    while (Cycles::rdtsc() < stopTime) {
        globalCount++;
    }
    printf("Core %d completed %lu increments\n", sched_getcpu(), globalCount.load());
}
