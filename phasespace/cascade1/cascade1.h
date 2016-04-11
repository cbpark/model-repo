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

bool isBMeson(const Pythia8::Particle& p) {
    int r1 = p.idAbs() % 10000;
    if (r1 < 511 || r1 > 545) {
        return false;
    }
    int r2 = r1 % 500;
    return r2 == 11 || r2 == 13 || r2 == 15 || r2 == 21 || r2 == 23 ||
           r2 == 25 || r2 == 31 || r2 == 33 || r2 == 35 || r2 == 41 ||
           r2 == 43 || r2 == 45;
}

#endif  // PHASESPACE_CASCADE1_H_
