package main

import "fmt"
import "time"
import "runtime"
import "os"
import "strconv"
import "sync/atomic"

const CORE_OCCUPANCY = 3
var stopTime time.Time
var globalCount uint64

// Artifical goroutine join
var done chan bool

func creator(counter uint64) {
    if time.Now().Before(stopTime) {
        go creator(counter + 1)
    } else {
       atomic.AddUint64(&globalCount, counter)
       done <- true
    }
}

func main() {
    args := os.Args
    if len(args) < 3 {
        fmt.Printf("Usage: ./GoThreadCreationScalability <NumCores> <Duration_Seconds>\n")
        os.Exit(1)
    }
    done = make(chan bool)

    numCores, _ := strconv.Atoi(args[1])
    numSeconds, _ := strconv.Atoi(args[2])

    runtime.GOMAXPROCS(numCores)

    startTime := time.Now()
    stopTime = startTime.Add(time.Duration(numSeconds) * time.Second)
    for i := 0; i < numCores * CORE_OCCUPANCY; i++ {
        go creator(1)
    }
    // wait for all extant goroutines to finish
    for i := 0; i < numCores * CORE_OCCUPANCY; i++ {
        <-done
    }
    duration := time.Since(startTime)
    fmt.Printf("Total Thread Creations: %v\n", globalCount)
    fmt.Printf("Total Time in Nanoseconds: %v\n", duration.Nanoseconds())
    fmt.Printf("Thread Creations Per Second: %f\n",
        float64(globalCount) / duration.Seconds())
}
