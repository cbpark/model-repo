CXXFLAGS := -g -O2 -Wall -Wextra -std=c++14 -I.
LDFLAGS  :=

# PYTHIA
PYTHIA   := $(shell pythia8-config --prefix)
CXXFLAGS += $(shell pythia8-config --cflags)
LDFLAGS  += -L$(PYTHIA)/lib -lpythia8 -Wl,-rpath $(PYTHIA)/lib

# FastJet
CXXFLAGS += $(shell fastjet-config --cxxflags)
LDFLAGS  += $(shell fastjet-config --libs)

# colevent
COLEVENT := /usr/local
CXXFLAGS += -I$(COLEVENT)/include/colevent
LDFLAGS  += -L$(COLEVENT)/lib -lcolevent -Wl,-rpath $(COLEVENT)/lib

.PHONY: all clean

all: qqbar_zh_parton.exe qqbar_zh_jet.exe

qqbar_zh_parton.exe: qqbar_zh_parton.o
	$(CXX) $(LDFLAGS) -o $@ $^

qqbar_zh_jet.exe: qqbar_zh_jet.o hadron_level_data.o jet_level_data.o
	$(CXX) $(LDFLAGS) -o $@ $^

clean:
	rm -f qqbar_zh_parton.exe qqbar_zh_jet.exe
