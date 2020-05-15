CXXFLAGS := -g -O2 -Wall -Wextra -std=c++14 -pedantic -I. $(CXXFLAGS)

# Pythia 8
CXXFLAGS +=   $(shell pythia8-config --cxxflags)
LDFLAGS  += -L$(shell pythia8-config --libdir) -lpythia8

# LHAPDF 6
CXXFLAGS += $(shell lhapdf-config --cflags)
LDFLAGS  += $(shell lhapdf-config --libs)

# FastJet
CXXFLAGS += $(shell fastjet-config --cxxflags)
LDFLAGS  += $(shell fastjet-config --libs)

# colevent (https://github.com/cbpark/colevent)
COLEVENT := /usr/local
CXXFLAGS += -I$(COLEVENT)/include/colevent
LDFLAGS  += -L$(COLEVENT)/lib -lcolevent -Wl,-rpath $(COLEVENT)/lib

EXE := pph2tt_2l_parton.exe
OBJ := pph2tt_2l_parton.o

.PHONY: all clean

all: pph2tt_2l_parton.exe

pph2tt_2l_parton.exe: pph2tt_2l_parton.o
	$(CXX) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(EXE)
