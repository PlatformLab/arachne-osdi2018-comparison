package main

import "time"
import "runtime"
import "os"
import "fmt"
import "sync"

const NUM_SAMPLES = 1000000

var latencies []time.Duration
var done chan bool
var producerSignal chan bool

var mutex sync.Mutex;
var productIsReady *sync.Cond;

// Time before notification
var beforeNotify time.Time


func producer() {
	for i := 0; i < NUM_SAMPLES; i++ {
        // Wait for consumer to be ready
         <-producerSignal
        mutex.Lock();
        beforeNotify = time.Now()
	    productIsReady.Signal();
        mutex.Unlock();
	}
    done <- true
}

func consumer() {
    mutex.Lock()
	for i := 0; i < NUM_SAMPLES; i++ {
        producerSignal <- true
        productIsReady.Wait();
        latencies = append(latencies, time.Since(beforeNotify))
	}
    mutex.Unlock()
    done <- true
}

func main() {
    done = make(chan bool)
    producerSignal = make(chan bool)
    latencies = make([]time.Duration, 0, NUM_SAMPLES)
    productIsReady = sync.NewCond(&mutex)
    runtime.GOMAXPROCS(2)

    go producer()
    go consumer()
    // Wait for both consumer and producer to finish
    <-done
    <-done

    // Output data
    fh, err := os.Create("data/Go Condition Variable")
    if err != nil {
        fmt.Printf("Failed to open output file. err = %s\n", err);
        return;
    }
    for i := 0; i < NUM_SAMPLES; i++ {
        fmt.Fprintf(fh, "%v\n", latencies[i].Nanoseconds())
    }
    fh.Close()
}
