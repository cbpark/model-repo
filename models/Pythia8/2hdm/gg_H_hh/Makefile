CXX = c++
CXXFLAGS += -Wall -g -O2 -I.

# PYTHIA
PYTHIA = /usr/local
CXXFLAGS += -I$(PYTHIA)/include
LDFLAGS += -L$(PYTHIA)/lib -lpythia8 -Wl,-rpath $(PYTHIA)/lib

.PHONY: all clean

all: gg_H_hh

gg_H_hh: gg_H_hh.cc
	$(CXX) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -rf *.dSYM
	rm -f gg_H_hh
