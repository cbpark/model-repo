#ifndef PHASESPACE_CASCADE1_H_
#define PHASESPACE_CASCADE1_H_

#include "CLHEF/lhef.h"
#include "Pythia8/Pythia.h"
#include "TLorentzVector.h"

lhef::Particle toLHEFParticle(int status, int mother1, int mother2,
                              const Pythia8::Particle& p) {
    return lhef::Particle(p.id(), status, mother1, mother2, 0, 0, p.px(),
                          p.py(), p.pz(), p.e(), p.m(), 0.0, 0.0);
}

TLorentzVector particleMomentum(const Pythia8::Particle& p) {
    return TLorentzVector(p.px(), p.py(), p.pz(), p.e());
}

Pythia8::Particle toParticle(int pid, const TLorentzVector& v) {
    return Pythia8::Particle(pid, 0, 0, 0, 0, 0, 0, 0, v.Px(), v.Py(), v.Pz(),
                             v.E(), v.M(), 0.0, 9.0);
}

#endif  // PHASESPACE_CASCADE1_H_
