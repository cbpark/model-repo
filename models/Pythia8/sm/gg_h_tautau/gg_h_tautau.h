#ifndef GG_H_TAUTAU_H_
#define GG_H_TAUTAU_H_

#include "Pythia8/Pythia.h"
#include "CLHEF/lhef.h"

lhef::Particle ToLHEFParticle(const int& status, const Pythia8::Particle& p) {
    return lhef::Particle(p.id(), status, 0, 0, 0, 0,
                          p.px(), p.py(), p.pz(), p.e(), p.m(),
                          0.0, 0.0);
}

#endif  // GG_H_TAUTAU_H_