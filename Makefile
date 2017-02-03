BINS=ThreadCreationTest ThreadYieldTest
GO_BINS=GoThreadCreate GoThreadYield

CXXFLAGS=-std=c++11 -O3 -Wall -Werror -Wformat=2 -Wextra -Wwrite-strings -Wno-unused-parameter -Wmissing-format-attribute -Wno-non-template-friend -Woverloaded-virtual -Wcast-qual -Wcast-align -Wconversion -fomit-frame-pointer
LIBS=-I../Arachne  -I../PerfUtils ../PerfUtils/libPerfUtils.a   -pthread


all: $(BINS) $(GO_BINS)

$(BINS) : % : %.cc
	g++  $(DEBUG) $(CXXFLAGS)  $< $(LIBS) -o $@

$(GO_BINS): % : %.go
	go build $<

clean:
	rm -f $(BINS) $(GO_BINS) *.log
