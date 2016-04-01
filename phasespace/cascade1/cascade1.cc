/*
 *  p p --> b1 + b2 --> h s + b2
 */
#include "TLorentzVector.h"
#include <cmath>
#include <iostream>

TLorentzVector initProtons(double beamE) {
    double protonM = 0.938272046;
    TLorentzVector beam(0.0, 0.0, std::sqrt(beamE * beamE - protonM * protonM),
                        beamE);
    TLorentzVector target(0.0, 0.0, 0.0, protonM);
    return beam + target;
}

int main() {
    TLorentzVector init = initProtons(400.0);
    init.Print();
}
