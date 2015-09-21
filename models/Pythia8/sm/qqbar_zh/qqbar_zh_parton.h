#ifndef QQBAR_ZH_PARTON_H_
#define QQBAR_ZH_PARTON_H_

#include "Pythia8/Pythia.h"
#include "CLHEF/lhef.h"

lhef::Particle toLHEFParticle(const int& status, const int& mother1,
                              const int& mother2, const Pythia8::Particle& p) {
    return lhef::Particle(p.id(), status, mother1, mother2, 0, 0, p.px(),
                          p.py(), p.pz(), p.e(), p.m(), 0.0, 0.0);
}

#endif  // QQBAR_ZH_PARTON_H_
