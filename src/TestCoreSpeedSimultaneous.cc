#include <stdlib.h>
#include <atomic>
#include <thread>
#include "PerfUtils/Cycles.h"
#include "PerfUtils/TimeTrace.h"
#include "PerfUtils/Util.h"
#include <unistd.h>


using PerfUtils::Cycles;
using PerfUtils::TimeTrace;

thread_local std::atomic<uint64_t> globalCount;

// Both of these times are in cycles
std::atomic<uint64_t> stopTime;
std::atomic<uint64_t> startTime;

void run(int coreId) {
    PerfUtils::Util::pinThreadToCore(coreId);
    uint64_t startCycles = Cycles::rdtsc();
    while (Cycles::rdtsc() < stopTime) {
        globalCount++;
    }
    uint64_t endCycles = Cycles::rdtsc();
    double time = Cycles::toSeconds(endCycles - startCycles);
    printf("Core %d performed %f increments/second\n", sched_getcpu(), static_cast<float>(globalCount.load()) / time);
}

/**
 * Pass in fixed number of cores, duration to run for in seconds.
 */
int main(int argc, const char** argv) {
    startTime = Cycles::rdtsc();
    uint64_t durationInCycles = Cycles::fromSeconds(5);
    stopTime = Cycles::rdtsc() + durationInCycles;
    for (int i = 0; i < 15; i++) {
        (new std::thread(run, i))->detach();
    }
    while (Cycles::rdtsc() < stopTime) {
        sleep(1);
    }
}
