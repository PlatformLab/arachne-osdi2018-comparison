package main

import "fmt"
import "time"
import "os"
import "strconv"

const numRuns = 10000

type  timeRecord struct{
    Ts time.Time
    Msg string
}

var timeStamps []timeRecord

func threadMain(done chan bool) {
    timeStamps = append(timeStamps, timeRecord{time.Now(), "Inside thread"})
    done <- true
}

func main() {
    timeStamps = make([]timeRecord, 0, numRuns*2)
    done := make(chan bool)
    dummy := 0
    limit, err := strconv.ParseInt(os.Args[1], 10, 64)

    if err != nil {
        return
    }

    for i := 0; i < numRuns; i++ {
        timeStamps = append(timeStamps, timeRecord{time.Now(), "Before creation"})
        go threadMain(done)
        for j := 0; j < int(limit); j++ {
            dummy += j
        }
        <-done
    }

    // Regularize
    regularizedTime := make([]time.Duration, numRuns*2)
    for i := 0; i < len(timeStamps) ; i++ {
        regularizedTime[i] = timeStamps[i].Ts.Sub(timeStamps[0].Ts)
    }

    // Fake timetraced
    fmt.Printf("%6d ns (+%6d ns): %s\n", 0, 0, timeStamps[0].Msg)
    for i := 1; i < len(timeStamps) ; i++ {
        fmt.Printf("%8d ns (+%6d ns): %s\n", regularizedTime[i], (regularizedTime[i] - regularizedTime[i-1]).Nanoseconds(), timeStamps[i].Msg)
    }
}
