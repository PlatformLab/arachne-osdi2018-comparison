package main

import "fmt"
import "time"
import "runtime"
import "os"

const numRuns = 1000000

var latencies []time.Duration

// Using this variable, we force task onto a different core, since main must poll.
// This technically violates the memory model described here, but we are using
// the channel to ensure correct ordering, and using this bool only to force
// the creator and createe to run in parallel.
//
// https://golang.org/ref/mem
var ran bool

// Artifical goroutine join
var done chan bool

func task(creationTime time.Time) {
    latencies = append(latencies, time.Since(creationTime))
    ran = true
    done <- true
}

func main() {
    done = make(chan bool)
    latencies = make([]time.Duration, 0, numRuns*2)
    runtime.GOMAXPROCS(2)

    for i := 0; i < numRuns; i++ {
        ran = false
        go task(time.Now())
        for ! ran { }
        <-done
    }

    // Output data
    fh, err := os.Create("data/Go Thread Creation")
    if err != nil {
        fmt.Printf("Failed to open output file. err = %s\n", err);
        return;
    }
    for i := 0; i < numRuns; i++ {
        fmt.Fprintf(fh, "%v\n", latencies[i].Nanoseconds())
    }
    fh.Close()
}
