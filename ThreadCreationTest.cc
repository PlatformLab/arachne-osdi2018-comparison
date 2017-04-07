#include <stdio.h>
#include <vector>
#include <thread>
#include <unistd.h>
#include <sched.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <atomic>

#include "Cycles.h"
#include "Stats.h"

using PerfUtils::Cycles;

#define NUM_SAMPLES 100

std::atomic<uint64_t> arrayIndex;
uint64_t latencies[NUM_SAMPLES];

void task(uint64_t creationTime) {
    uint64_t latency = Cycles::rdtsc() - creationTime;
    latencies[arrayIndex++] = latency;
}

int main() {
    // Measure the thread creation overhead in the creating thread.
    for (int i = 0; i < NUM_SAMPLES; i++) {
        // Note that there is a risk of Cycles::rdtsc running later than usual.
        std::thread(task, Cycles::rdtsc()).join();
    }
    if (arrayIndex != NUM_SAMPLES) abort();
    printStatistics("Thread Creation Latency", latencies, NUM_SAMPLES, "data");
    printf("CYCLES_PER_SECOND: %lf\n", Cycles::perSecond());
}
