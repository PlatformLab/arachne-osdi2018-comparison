#include <stdlib.h>
#include <atomic>
#include "PerfUtils/Cycles.h"
#include "PerfUtils/TimeTrace.h"
#include "Arachne/Arachne.h"
#include "Arachne/DefaultCorePolicy.h"

using PerfUtils::Cycles;

// This benchmark features a central dispatch thread creating threads as fast
// as possible to simulate a wave of requests coming into a server, and
// measures the number of requests completed after a user-specified interval
// using user-specified number of cores.

// Amount of time each worker should run for, in microseconds.
#define THREAD_RUN_DURATION 1

/**
  * The number of threads that finished executing in a given amount of time.
  * This would be more efficient as a collection of thread_local variables that
  * were summed at completion, but Go does not have TLS, so we keep things
  * simple for now.
  */
std::atomic<uint64_t> completedRequests;

// Both of these times are in cycles
std::atomic<uint64_t> stopTime;
std::atomic<uint64_t> startTime;

// This actual time experiment ran for.
std::atomic<double> duration;

/**
 * Spin for duration cycles and record completion before exiting.
 */
void
fixedWork(uint64_t duration) {
    uint64_t startTime = Cycles::rdtsc();
    uint64_t stop = startTime + duration;

    // If we've run out of time for the experiment, we should exit without recording a completion.
    if (stop > stopTime) {
        return;
    }

    // Spin until it's time to stop.
    while (Cycles::rdtsc() < stop)
        ;
    completedRequests++;
}

/**
  * We run this thread within Arachne to create symmetry with Golang, since
  * Go's main function is a goroutine.
  */
void
dispatch(int numSeconds) {
    startTime = Cycles::rdtsc();
    stopTime = Cycles::rdtsc() + Cycles::fromSeconds(numSeconds);

    const uint64_t durationInCycles = Cycles::fromMicroseconds(THREAD_RUN_DURATION);

    while (Cycles::rdtsc() < stopTime) {
        // Create as fast as possible, ignoring failures.
        Arachne::createThread(fixedWork, durationInCycles);
    }
    duration = Cycles::toSeconds(Cycles::rdtsc() - startTime);
    Arachne::shutDown();
}

int main(int argc, const char** argv){
    if (argc < 3) {
        printf("Usage: ./ArachneSingleCreatorScalability.cc <NumCores> <Duration_Seconds>\n");
        exit(1);
    }
    int numCores = atoi(argv[1]);
    int numSeconds = atoi(argv[2]);

    Arachne::minNumCores = numCores;
    Arachne::maxNumCores = numCores;
    Arachne::init(&argc, argv);

    Arachne::createThreadWithClass(Arachne::DefaultCorePolicy::EXCLUSIVE,
            dispatch, numSeconds);
    Arachne::waitForTermination();

    printf("%d,%d,%lu\n", numSeconds, numCores, static_cast<uint64_t>(
                static_cast<double>(completedRequests.load()) / duration.load()));
}
