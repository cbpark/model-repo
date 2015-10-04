CXX = c++
CXXFLAGS += -Wall -Wextra -g -O2 -I.

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

# CLHCO (https://github.com/cbpark/CLHCO)
CLHCO = /usr/local

# FastJet
FASTJET = /usr/local

.PHONY: all clean

all: qqbar_zh_parton qqbar_zh_jet

qqbar_zh_parton: qqbar_zh_parton.cc
	$(CXX) $^ -o $@ $(CXXFLAGS) $(CXX11) $(LDFLAGS) \
	-I$(CLHEF)/include -L$(CLHEF)/lib -lCLHEF -Wl,-rpath $(CLHEF)/lib

qqbar_zh_jet: qqbar_zh_jet.o hadron_level_data.o jet_level_data.o
	$(CXX) $(LDFLAGS) $^ -o $@ \
	-L$(CLHCO)/lib -lCLHCO -Wl,-rpath,$(CLHCO)/lib \
	-L$(FASTJET)/lib -lfastjettools -lfastjet -lfastjetplugins -lsiscone_spherical -lsiscone \
	-Wl,-rpath,$(FASTJET)/lib

qqbar_zh_jet.o: qqbar_zh_jet.cc
	$(CXX) $(CXXFLAGS) $(CXX11) -I$(FASTJET)/include -I$(CLHCO)/include -o $@ -c $<

hadron_level_data.o: hadron_level_data.cc
	$(CXX) $(CXXFLAGS) $(CXX11) -o $@ -c $<

jet_level_data.o: jet_level_data.cc
	$(CXX) $(CXXFLAGS) $(CXX11) -I$(FASTJET)/include -o $@ -c $<

clean:
	rm -rf *.dSYM *.o
	rm -f qqbar_zh_parton qqbar_zh_jet
