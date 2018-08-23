CXXFLAGS=-std=c++11 -O3 -g -Wall -Werror -Wformat=2 -Wextra -Wwrite-strings -Wno-unused-parameter -Wmissing-format-attribute -Wno-non-template-friend -Woverloaded-virtual -Wcast-qual -Wcast-align -Wconversion -fomit-frame-pointer

# Output directories
SRC_DIR = src
BIN_DIR = bin

# Dependencies
PERFUTILS=../PerfUtils
ARACHNE=../arachne-all/Arachne
COREARBITER=../arachne-all/CoreArbiter

LIBS=-I$(ARACHNE)/include -I$(PERFUTILS)/include -I$(COREARBITER)/include  $(ARACHNE)/lib/libArachne.a -L$(COREARBITER)/lib -lCoreArbiter $(PERFUTILS)/lib/libPerfUtils.a  -lpcrecpp -pthread

BINS=ThreadCreationTest ThreadYieldTest StdThreadCVWakeup ThreadCreationScalability ExtractStats \
	ArachneCreationScalability ArachneSingleCreatorScalability SingleCreatorScalability \
	GoThreadCreate GoThreadYield GoThreadCV GoThreadCreationScalability GoSingleCreatorScalability

FULL_BINS = $(patsubst %,$(BIN_DIR)/%,$(BINS))

all: $(FULL_BINS)

$(BIN_DIR)/% : $(SRC_DIR)/%.cc | $(BIN_DIR)
	g++  $(CXXFLAGS)  $< $(LIBS) -o $@

$(BIN_DIR)/% : $(SRC_DIR)/%.go | $(BIN_DIR)
	go build  -o $@ $<

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)
