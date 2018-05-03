package main

import "fmt"
import "time"
import "runtime"
import "os"
import "strconv"
import "sync/atomic"

// This benchmark features a central dispatch thread creating threads as fast
// as possible to simulate a wave of requests coming into a server, and
// measures the number of requests completed after a user-specified interval
// using user-specified number of cores.

// Amount of time each worker should run for, in microseconds.
const THREAD_RUN_DURATION = 1

var startTime time.Time
var stopTime time.Time


/**
  * The number of threads that finished executing in a given amount of time.
  */
var completedRequests uint64

// This actual time experiment ran for.
var duration time.Duration

/**
 * Spin for duration cycles and record completion before exiting.
 */
func fixedWork(duration time.Duration) {
    start := time.Now()
    stop  := start.Add(duration)

    // If we've run out of time for the experiment, we should exit without recording a completion.
    if stop.After(stopTime) {
        return;
    }

    // Spin until it's time to stop.
    for time.Now().Before(stop) {
    }
    atomic.AddUint64(&completedRequests, 1)
}

/**
  * We run this thread within Arachne to create symmetry with Golang, since
  * Go's main function is a goroutine.
  */
func dispatch(numSeconds int) {
    startTime = time.Now()
    stopTime = startTime.Add(time.Duration(numSeconds) * time.Second)

    const workDuration = time.Duration(THREAD_RUN_DURATION) * time.Microsecond

    for time.Now().Before(stopTime) {
        // Create as fast as possible, ignoring failures.
        go fixedWork(workDuration);
    }
    duration = time.Now().Sub(startTime)
}

func main(){
    // Force Go to do Cross-Core creations.
    runtime.LockOSThread()
    args := os.Args
    if len(args) < 3 {
        fmt.Printf("Usage: ./GoSingleCreatorScalability <NumCores> <Duration_Seconds>\n");
        os.Exit(1)
    }

    numCores, _ := strconv.Atoi(args[1])
    numSeconds, _ := strconv.Atoi(args[2])

    runtime.GOMAXPROCS(numCores)

    dispatch(numSeconds)

    fmt.Printf("%d,%d,%v\n", numSeconds, numCores, uint64(float64(completedRequests) / duration.Seconds()));
}
