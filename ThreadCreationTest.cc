#include <stdio.h>
#include <vector>
#include <thread>
#include <unistd.h>
#include <sched.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "Cycles.h"
#include "TimeTrace.h"


using PerfUtils::Cycles;

#define NUM_RUNS 10000000

// TimeTrace cannot be used for this experiment because it uses thread-local
// buffers which dissappear with each thread. Thus, we need to roll a poor
// man's version for this experiment.
uint64_t records[NUM_RUNS * 2];

void threadFunc(int index) {
    uint64_t timestamp = Cycles::rdtsc();
    records[index] = timestamp;
}

int main(){
    // Measure the thread creation overhead in the creating thread.
    for (int i = 0; i < NUM_RUNS; i++) {
        records[i] = Cycles::rdtsc();
        std::thread(threadFunc, i + NUM_RUNS).join();
    }

    // Regularize 
    uint64_t startTime = records[0];
    for (int i = 0; i < NUM_RUNS * 2; i++) {
        records[i] -= startTime;
    }

    // Fake timetraced
    printf("%6lu ns (+%6lu ns): %s\n", 0L, 0L, "Before creation");
    printf("%6lu ns (+%6lu ns): %s\n", Cycles::toNanoseconds(records[NUM_RUNS]), Cycles::toNanoseconds(records[NUM_RUNS]), "Inside thread");
    for (int i = 1; i < NUM_RUNS; i++) {
        printf("%6lu ns (+%6lu ns): %s\n", Cycles::toNanoseconds(records[i]),
                Cycles::toNanoseconds(records[i] - records[i + NUM_RUNS - 1]), "Before creation");
        printf("%6lu ns (+%6lu ns): %s\n", Cycles::toNanoseconds(records[i + NUM_RUNS]),
                Cycles::toNanoseconds(records[i + NUM_RUNS] - records[i]), "Inside thread");
    }

    fflush(stdout);
}
