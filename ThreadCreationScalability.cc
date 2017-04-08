#include <stdlib.h>
#include <atomic>
#include <thread>
#include "Cycles.h"
#include "Semaphore.h"

#define CORE_OCCUPANCY 3

using PerfUtils::Cycles;

static std::atomic<uint64_t> globalCount;

// Both of these times are in cycles
std::atomic<uint64_t> stopTime;
std::atomic<uint64_t> startTime;

// This time is in seconds
std::atomic<double> duration;

Semaphore done;

void creator(uint64_t counter) {
    if (Cycles::rdtsc() < stopTime) {
        std::thread(creator, counter + 1).detach();
    } else {
        globalCount += counter;
        done.notify();
    }
}

/**
 * Pass in fixed number of cores, duration to run for in seconds.
 */
int main(int argc, const char** argv) {
    if (argc < 3) {
        printf("Usage: ./ThreadCreationScalability <NumCores> <Duration_Seconds>\n");
        exit(1);
    }
    int numCores = atoi(argv[1]);
    int numSeconds = atoi(argv[2]);

	startTime = Cycles::rdtsc();
    uint64_t durationInCycles = Cycles::fromSeconds(numSeconds);
    stopTime = Cycles::rdtsc() + durationInCycles;
	for (int i = 0; i < numCores * CORE_OCCUPANCY; i++)
		std::thread(creator, 1).detach();
    // Wait for all threads to finish, using a semaphor
	for (int i = 0; i < numCores * CORE_OCCUPANCY; i++)
        done.wait();
	duration = Cycles::toSeconds(Cycles::rdtsc() - startTime);

    // Number of Seconds,Number Of Cores,Creations Per Second
	printf("%d,%d,%lu\n", numSeconds, numCores,
            static_cast<uint64_t>(static_cast<double>(globalCount.load()) / duration));
    return 0;
}
