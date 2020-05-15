CXXFLAGS := -g -O2 -Wall -I.
LDFLAGS  := -O2 -m64

# PYTHIA
PYTHIA   := $(shell pythia8-config --prefix)
CXXFLAGS += $(shell pythia8-config --cflags)
LDFLAGS  += -L$(PYTHIA)/lib -lpythia8 -Wl,-rpath $(PYTHIA)/lib

# HepMC
HEPMC    ?= /usr/local
CXXFLAGS += -I$(HEPMC)/include
LDFLAGS  += -L$(HEPMC)/lib -lHepMC -Wl,-rpath $(HEPMC)/lib

.PHONY: all clean

all: LHE2HepMC.exe

%.exe: %.o
	$(CXX) $(LDFLAGS) -o $@ $^

clean:
	rm -f LHE2HepMC.o LHE2HepMC.exe
