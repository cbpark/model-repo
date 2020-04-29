CXXFLAGS := -g -O2 -Wall -std=c++14
LDFLAGS  := -O2

# ROOT
CXXFLAGS += $(shell root-config --cflags)
LDFLAGS  += $(shell root-config --libs)

# colevent (https://github.com/cbpark/colevent)
COLEVENT := /usr/local
CXXFLAGS += -I$(COLEVENT)/include/colevent
LDFLAGS  += -L$(COLEVENT)/lib -lcolevent -Wl,-rpath $(COLEVENT)/lib

.PHONY: all clean

all: antler.exe

antler.exe: antler.o
	$(CXX) $(LDFLAGS) -o $@ $^

clean:
	rm -f antler.exe antler.o
