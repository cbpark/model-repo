#ifndef JET_LEVEL_DATA_H_
#define JET_LEVEL_DATA_H_

#include "hadron_level_data.h"

#include "fastjet/ClusterSequence.hh"
#include "fastjet/PseudoJet.hh"

#include <utility>
#include <vector>

struct JetLevelData {
    std::vector<fastjet::PseudoJet> photons;
    std::vector<fastjet::PseudoJet> electrons;
    std::vector<fastjet::PseudoJet> muons;
    std::vector<fastjet::PseudoJet> taus;
    std::vector<fastjet::PseudoJet> jets;
    std::vector<fastjet::PseudoJet> bJets;
    std::pair<double, double> met;
};

int charge(const fastjet::PseudoJet& j);

JetLevelData reconstructObjects(const HadronLevelData& hadronData,
                                double coneR);

#endif  // JET_LEVEL_DATA_H_
