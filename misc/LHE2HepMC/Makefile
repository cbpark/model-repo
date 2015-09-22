CXX = c++
CXXFLAGS += -Wall -g -O2 -I.

# PYTHIA
PYTHIA = /usr/local
CXXFLAGS += -I$(PYTHIA)/include
LDFLAGS += -L$(PYTHIA)/lib -lpythia8 -Wl,-rpath $(PYTHIA)/lib

# HepMC
HEPMC = /usr/local
CXXFLAGS += -I$(HEPMC)/include
LDFLAGS += -L$(HEPMC)/lib -lHepMC -Wl,-rpath $(HEPMC)/lib

.PHONY: all clean

all: LHE2HepMC

LHE2HepMC: LHE2HepMC.cc
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -rf *.dSYM
	rm -f LHE2HepMC
