CXX := c++
CXXFLAGS += -Wall -Wextra -g -O2 -I.

# Flags for C++11
CXX11 := -std=c++11
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	CXX11 += -stdlib=libc++
endif

# PYTHIA
PYTHIA=/usr/local
CXXFLAGS += -I$(PYTHIA)/include
LDFLAGS += -L$(PYTHIA)/lib -lpythia8 -Wl,-rpath $(PYTHIA)/lib

# CLHEF (https://github.com/cbpark/CLHEF)
CLHEF = /usr/local

# ROOT
CXXFLAGS += `root-config --cflags`
LDFLAGS += `root-config --libs`

.PHONY: all clean

all: cascade1

cascade1: cascade1.cc
	$(CXX) $^ -o $@ $(CXXFLAGS) $(CXX11) $(LDFLAGS) \
	-I$(CLHEF)/include -L$(CLHEF)/lib -lCLHEF -Wl,-rpath $(CLHEF)/lib

clean:
	rm -f cascade1
	rm -rf *.dSYM
