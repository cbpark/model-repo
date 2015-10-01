#ifndef MODELS_PYTHIA8_SM_QQBAR_ZH_JET_LEVEL_DATA_H_
#define MODELS_PYTHIA8_SM_QQBAR_ZH_JET_LEVEL_DATA_H_

#include <vector>
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "hadron_level_data.h"

struct JetLevelData {
    std::vector<fastjet::PseudoJet> photons;
    std::vector<fastjet::PseudoJet> electrons;
    std::vector<fastjet::PseudoJet> muons;
    std::vector<fastjet::PseudoJet> taus;
    std::vector<fastjet::PseudoJet> jets;
    std::vector<fastjet::PseudoJet> bJets;
};

JetLevelData reconstructObjects(const HadronLevelData& hadrons);

#endif  // MODELS_PYTHIA8_SM_QQBAR_ZH_JET_LEVEL_DATA_H_
