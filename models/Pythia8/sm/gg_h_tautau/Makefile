# Makefile is a part of the PYTHIA event generator.
# Copyright (C) 2015 Torbjorn Sjostrand.
# PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
# Please respect the MCnet Guidelines, see GUIDELINES for details.
# Author: Philip Ilten, September 2014.
#
# This is is the Makefile used to build PYTHIA examples on POSIX systems.
# Example usage is:
#     make main01
# For help using the make command please consult the local system documentation,
# i.e. "man make" or "make --help".

################################################################################
# VARIABLES: Definition of the relevant variables from the configuration script.
################################################################################

# Include the configuration.
-include Makefile.inc

# Handle GZIP support.
ifeq ($(GZIP_USE),true)
  CXX_COMMON+= -DGZIPSUPPORT -I$(GZIP_INCLUDE)
  CXX_COMMON+= -L$(GZIP_LIB) -Wl,-rpath $(GZIP_LIB) -lz
endif

# Check distribution (use local version first, then installed version).
ifneq ("$(wildcard ../lib/libpythia8.a)","")
  PREFIX_LIB=../lib
  PREFIX_INCLUDE=../include
endif
CXX_COMMON:=-I$(PREFIX_INCLUDE) $(CXX_COMMON) -Wl,-rpath $(PREFIX_LIB) -ldl

# CLHEF (https://github.com/cbpark/CLHEF)
CLHEF = /usr/local

# Flags for C++11
CXX11 = -std=c++11 -stdlib=libc++

################################################################################
# RULES: Definition of the rules used to build the PYTHIA examples.
################################################################################

# Rules without physical targets (secondary expansion for specific rules).
.SECONDEXPANSION:
.PHONY: all clean

# All targets (no default behavior).
all: \
	gg_h_tautau

# The Makefile configuration.
Makefile.inc:
	$(error Error: PYTHIA must be configured, please run "./configure"\
                in the top PYTHIA directory)

# PYTHIA libraries.
$(PREFIX_LIB)/libpythia8.a :
	$(error Error: PYTHIA must be built, please run "make"\
                in the top PYTHIA directory)

gg_h_tautau: $$@.cc \
	$(PREFIX_LIB)/libpythia8.a
	$(CXX) $^ -o $@ $(CXX_COMMON) $(CXX11) -I$(CLHEF) -L$(CLHEF) -lCLHEF

# Clean.
clean:
	rm -f gg_h_tautau;
	rm -f *~; rm -f \#*
