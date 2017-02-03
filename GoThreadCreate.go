package main

import "fmt"
import "time"

const numRuns = 100000

type  timeRecord struct{
    Ts time.Time
    Msg string
}


var timeStamps []timeRecord
func threadMain(a, b int) {
    timeStamps = append(timeStamps, timeRecord{time.Now(), "Inside thread"})
}

func main() {
    timeStamps = make([]timeRecord, 0, numRuns*2)
    for i := 0; i < numRuns; i++ {
        timeStamps = append(timeStamps, timeRecord{time.Now(), "Before creation"})
        go threadMain(0, 1)
        time.Sleep(100 * time.Microsecond)
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
