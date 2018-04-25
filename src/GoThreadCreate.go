package main

import "fmt"
import "time"
import "runtime"
import "os"

const NUM_SAMPLES = 1000000

var latencies []time.Duration

// Artifical goroutine join
var done chan bool

func task(creationTime time.Time) {
    latencies = append(latencies, time.Since(creationTime))
    done <- true
}

func main() {
    done = make(chan bool)
    latencies = make([]time.Duration, 0, NUM_SAMPLES)
    runtime.GOMAXPROCS(2)

    for i := 0; i < NUM_SAMPLES; i++ {
        go task(time.Now())
        <-done
    }

    // Output data
    fh, err := os.Create("data/Go Thread Creation")
    if err != nil {
        fmt.Printf("Failed to open output file. err = %s\n", err);
        return;
    }
    for i := 0; i < NUM_SAMPLES; i++ {
        fmt.Fprintf(fh, "%v\n", latencies[i].Nanoseconds())
    }
    fh.Close()
}
