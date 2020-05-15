#ifndef MODELS_PYTHIA8_SM_QQBAR_ZH_QQBAR_ZH_JET_H_
#define MODELS_PYTHIA8_SM_QQBAR_ZH_QQBAR_ZH_JET_H_

#include "fastjet/PseudoJet.hh"
#include "lhco/lhco.h"
#include "jet_level_data.h"

lhco::Object toLHCOObj(const fastjet::PseudoJet& j, int typ, int ntrk,
                       int btag) {
    double eta = j.eta();
    double phi = j.phi_std();
    double pt = j.pt();
    double m = j.m();
    return lhco::Object(typ, eta, phi, pt, m, ntrk, btag, 0.0);
}

lhco::Objects jetLevelToLHCO(const JetLevelData& jetLevel) {
    lhco::Objects objs;
    for (const auto& p : jetLevel.photons) {
        objs.push_back(toLHCOObj(p, 0, 1, 0));
    }
    for (const auto& p : jetLevel.electrons) {
        int q = charge(p);
        objs.push_back(toLHCOObj(p, 1, q, 0));
    }
    for (const auto& p : jetLevel.muons) {
        int q = charge(p);
        objs.push_back(toLHCOObj(p, 2, q, 0));
    }
    for (const auto& p : jetLevel.taus) {
        objs.push_back(toLHCOObj(p, 3, 0, 0));
    }

    auto allJets = jetLevel.jets;
    allJets.insert(allJets.end(), jetLevel.bJets.begin(), jetLevel.bJets.end());
    allJets = fastjet::sorted_by_pt(allJets);
    for (const auto& p : allJets) {
        int btag = p.user_index() == 5 ? 1 : 0;
        objs.push_back(toLHCOObj(p, 4, 0, btag));
    }

    double metPt = jetLevel.met.first;
    double metPhi = jetLevel.met.second;
    objs.push_back(lhco::Object(6, 0.0, metPhi, metPt, 0.0, 0, 0, 0));

    return objs;
}

#endif  // MODELS_PYTHIA8_SM_QQBAR_ZH_QQBAR_ZH_JET_H_
