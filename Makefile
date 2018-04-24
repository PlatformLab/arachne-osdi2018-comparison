BINS=ThreadCreationTest ThreadYieldTest StdThreadCVWakeup ThreadCreationScalability ExtractStats
GO_BINS=GoThreadCreate GoThreadYield GoThreadCV GoThreadCreationScalability

CXXFLAGS=-std=c++11 -O3 -g -Wall -Werror -Wformat=2 -Wextra -Wwrite-strings -Wno-unused-parameter -Wmissing-format-attribute -Wno-non-template-friend -Woverloaded-virtual -Wcast-qual -Wcast-align -Wconversion -fomit-frame-pointer
LIBS=-I../Arachne  -I../PerfUtils/include ../PerfUtils/lib/libPerfUtils.a   -pthread


all: $(BINS) $(GO_BINS)

$(BINS) : % : %.cc
	g++  $(DEBUG) $(CXXFLAGS)  $< $(LIBS) -o $@

$(GO_BINS): % : %.go
	go build $<

clean:
	rm -f $(BINS) $(GO_BINS) *.log
