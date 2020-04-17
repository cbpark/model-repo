CXXFLAGS := -g -O2 -Wall -Wextra -pedantic -I. $(CXXFLAGS)

# Pythia 8
CXXFLAGS += $(shell pythia8-config --cxxflags)
LDFLAGS  += $(shell pythia8-config --ldflags)

EXE    := pph2hpw.exe pph2hpw_2l.exe
EXESRC := $(patsubst %.exe,%.cc,$(EXE))
EXEOBJ := $(EXESRC:.cc=.o)

.PHONY: all clean

all: $(EXE)

%.exe: %.o
	$(CXX) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(EXE)
