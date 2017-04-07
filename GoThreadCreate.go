package main

import "fmt"
import "time"
import "runtime"

import "os"
//import "strconv"

const numRuns = 1000000

var latencies []time.Duration

// Artificial join mechanism
var done chan bool

func task(creationTime time.Time) {
    latencies = append(latencies, time.Since(creationTime))
    done <- true
}

func main() {
    latencies = make([]time.Duration, 0, numRuns*2)
    done = make(chan bool)
    runtime.GOMAXPROCS(2)

    for i := 0; i < numRuns; i++ {
        go task(time.Now())
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
