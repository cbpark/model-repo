#ifndef MODELS_PYTHIA8_SM_QQBAR_ZH_QQBAR_ZH_JET_H_
#define MODELS_PYTHIA8_SM_QQBAR_ZH_QQBAR_ZH_JET_H_

#include "fastjet/PseudoJet.hh"
#include "CLHCO/lhco.h"

lhco::Object toLHCOObj(const fastjet::PseudoJet& j, int typ, int ntrk,
                       int btag) {
    double eta = j.eta();
    double phi = j.phi_std();
    double pt = j.pt();
    double m = j.m();
    return lhco::Object(typ, eta, phi, pt, m, ntrk, btag, 0.0);
}

#endif  // MODELS_PYTHIA8_SM_QQBAR_ZH_QQBAR_ZH_JET_H_
