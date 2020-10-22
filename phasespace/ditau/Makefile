CXXFLAGS := -g -O2 -Wall -std=c++14
LDFLAGS  := -O2

# ROOT
CXXFLAGS += $(shell root-config --cflags)
LDFLAGS  += $(shell root-config --libs)

# colevent (https://github.com/cbpark/colevent)
COLEVENT ?= /usr/local
CXXFLAGS += -I$(COLEVENT)/include/colevent
LDFLAGS  += -L$(COLEVENT)/lib -lcolevent -Wl,-rpath $(COLEVENT)/lib

EXE    := ditau_2.exe ditau_3.exe ditau_4.exe
EXESRC := $(patsubst %.exe,%.cc,$(EXE))
EXEOBJ := $(EXESRC:.cc=.o)

.PHONY: all clean

all: $(EXE)

%.exe: %.o
	$(CXX) $(LDFLAGS) -o $@ $^

clean::
	rm -f $(EXEOBJ) $(EXE)
