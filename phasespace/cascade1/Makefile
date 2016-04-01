CXX := c++
CXXFLAGS += -Wall -Wextra -g -O2 -I.

# Flags for C++11
CXX11 := -std=c++11
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
	CXX11 += -stdlib=libc++
endif

# ROOT
CXXFLAGS += `root-config --cflags`
LDFLAGS += `root-config --libs`

.PHONY: all clean

all: cascade1

cascade1: cascade1.cc
	$(CXX) $^ -o $@ $(CXXFLAGS) $(CXX11) $(LDFLAGS)

clean:
	rm -f cascade1
	rm -rf *.dSYM
