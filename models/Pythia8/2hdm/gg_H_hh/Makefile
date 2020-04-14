CXXFLAGS := -g -O2 -Wall -Wextra -pedantic $(CXXFLAGS)

# PYTHIA
CXXFLAGS += $(shell pythia8-config --cxxflags)
LDFLAGS  += $(shell pythia8-config --ldflags)

.PHONY: all clean

all: gg_H_hh.exe

gg_H_hh.exe: gg_H_hh.o
	$(CXX) $(LDFLAGS) -o $@ $^

clean:
	rm -f gg_H_hh.o gg_H_hh.exe
