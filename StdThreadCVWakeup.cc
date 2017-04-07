#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "Cycles.h"
#include "Util.h"
#include "Stats.h"

#define NUM_SAMPLES 1000000

using PerfUtils::Cycles;
std::atomic<uint64_t> arrayIndex;
uint64_t latencies[NUM_SAMPLES];

std::atomic<uint64_t> beforeNotify;

std::condition_variable productIsReady;
std::mutex mutex;

std::atomic<bool> producerHasStarted;

void producer() {
    producerHasStarted = true;
	for (int i = 0; i < NUM_SAMPLES; i++) {
        while (producerHasStarted);
        producerHasStarted = true;
        mutex.lock();
        beforeNotify = Cycles::rdtsc();
	    productIsReady.notify_one();
        mutex.unlock();
	}
}

void consumer() {
    std::unique_lock<std::mutex> guard(mutex);
	for (int i = 0; i < NUM_SAMPLES; i++) {
        producerHasStarted = false;
        productIsReady.wait(guard);
        latencies[arrayIndex++] = Cycles::rdtsc() - beforeNotify;
	}
}

int main(int argc, const char** argv){
    // Add some work
    std::thread producerThread(producer);
    while(!producerHasStarted);
    std::thread consumerThread(consumer);
    producerThread.join();
    consumerThread.join();

    if (arrayIndex != NUM_SAMPLES) abort();
    for (int i = 0; i < NUM_SAMPLES; i++)
        latencies[i] = Cycles::toNanoseconds(latencies[i]);
    printStatistics("Condition Variable Wakeup", latencies, NUM_SAMPLES, "data");
    printf("CYCLES_PER_SECOND: %lf\n", Cycles::perSecond());
    return 0;
}
