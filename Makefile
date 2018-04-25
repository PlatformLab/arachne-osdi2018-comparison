CXXFLAGS=-std=c++11 -O3 -g -Wall -Werror -Wformat=2 -Wextra -Wwrite-strings -Wno-unused-parameter -Wmissing-format-attribute -Wno-non-template-friend -Woverloaded-virtual -Wcast-qual -Wcast-align -Wconversion -fomit-frame-pointer

# Output directories
SRC_DIR = src
BIN_DIR = bin

# Dependencies
PERFUTILS=../PerfUtils
LIBS=-I../Arachne  -I$(PERFUTILS)/include $(PERFUTILS)/lib/libPerfUtils.a  -pthread

BINS=ThreadCreationTest ThreadYieldTest StdThreadCVWakeup ThreadCreationScalability ExtractStats \
	GoThreadCreate GoThreadYield GoThreadCV GoThreadCreationScalability

FULL_BINS = $(patsubst %,$(BIN_DIR)/%,$(BINS))

all: $(FULL_BINS)

$(BIN_DIR)/% : $(SRC_DIR)/%.cc | $(BIN_DIR)
	g++  $(CXXFLAGS)  $< $(LIBS) -o $@

$(BIN_DIR)/% : $(SRC_DIR)/%.go | $(BIN_DIR)
	go build  -o $@ $<

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(BIN_DIR)
