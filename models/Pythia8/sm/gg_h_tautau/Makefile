CXX = c++
CXXFLAGS += -Wall -g -O2 -I.

# Flags for C++11
CXX11 = -std=c++11
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	CXX11 += -stdlib=libc++
endif

# PYTHIA
PYTHIA = /usr/local
CXXFLAGS += -I$(PYTHIA)/include
LDFLAGS += -L$(PYTHIA)/lib -lpythia8 -Wl,-rpath $(PYTHIA)/lib

# CLHEF (https://github.com/cbpark/CLHEF)
CLHEF = /usr/local

.PHONY: all clean

all: gg_h_tautau_parton

gg_h_tautau_parton: gg_h_tautau_parton.cc
	$(CXX) $^ -o $@ $(CXXFLAGS) $(CXX11) $(LDFLAGS) \
	-I$(CLHEF)/include -L$(CLHEF)/lib -lCLHEF -Wl,-rpath $(CLHEF)/lib

clean:
	rm -f gg_h_tautau_parton
