package main

import "fmt"
import "time"
import "runtime"

type  timeRecord struct{
    Ts time.Time
    Msg string
    Index int64
}

func threadMain(start, end int64) {
    startTime := time.Now()
    for i := start; i < end; i+=2 {
        runtime.Gosched()
    }
    timePerYield := (time.Now().Sub(startTime)).Nanoseconds() / (end - start);
    fmt.Printf("%v\n", timePerYield);
}

func main() {
    go threadMain(1, 99999999)
    go threadMain(2,100000000)
    time.Sleep(30 * time.Second)
}
