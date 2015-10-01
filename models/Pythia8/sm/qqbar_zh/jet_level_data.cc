#include "jet_level_data.h"
#include <algorithm>
#include <iterator>
#include <map>
#include <memory>
#include <vector>
#include "Pythia8/Pythia.h"
#include "hadron_level_data.h"

using Pythia8::Particle;
using fastjet::PseudoJet;
using PseudoJets = std::vector<PseudoJet>;

PseudoJet py2FjInput(const Particle& p) {
    fastjet::PseudoJet fjInput(p.px(), p.py(), p.pz(), p.e());
    fjInput.set_user_index(p.id());
    return fjInput;
}

PseudoJets isolatedObjects(const std::map<int, Particle>& ls) {
    PseudoJets ljets;
    for (const auto& l : ls) {
        auto lp = l.second;
        ljets.push_back(py2FjInput(lp));
    }
    return fastjet::sorted_by_pt(ljets);
}

PseudoJets getJets(const HadronLevelData& hadrons) {
    PseudoJets particles;
    for (const auto& h : hadrons.others) particles.push_back(py2FjInput(h));

    fastjet::JetDefinition jetDef(fastjet::antikt_algorithm, 0.5,
                                  fastjet::E_scheme, fastjet::Best);
    fastjet::ClusterSequence clusterSeq(particles, jetDef);

    PseudoJets jets = clusterSeq.inclusive_jets(5.0);
    jets.erase(std::remove_if(jets.begin(), jets.end(), [](const PseudoJet& j) {
                   return std::fabs(j.eta()) > 2.5;
               }), jets.end());

    return jets;
};

std::pair<PseudoJets, PseudoJets> bTag(const std::vector<Particle>& bquarks,
                                       const PseudoJets& jets, double coneR) {
    PseudoJets candidates = jets;
    if (!candidates.empty()) {
        for (const auto& b : bquarks) {
            if (b.pT() < 5.0 || fabs(b.eta()) > 2.5) continue;

            PseudoJet bquark(b.px(), b.py(), b.pz(), b.e());
            auto j = std::min_element(
                candidates.begin(), candidates.end(),
                [&bquark](const PseudoJet& j1, const PseudoJet& j2) {
                    return j1.delta_R(bquark) < j2.delta_R(bquark);
                });
            if (j->delta_R(bquark) < coneR)
                j->set_user_index(j->user_index() + 1);
        }
    }

    PseudoJets bJets, normalJets;
    std::partition_copy(candidates.begin(), candidates.end(),
                        std::back_inserter(bJets),
                        std::back_inserter(normalJets),
                        [](const PseudoJet& j) { return j.user_index() > 0; });
    return std::make_pair(bJets, normalJets);
}

// bool tauTag(const PseudoJet& jet, double pTthres) {
//     auto constituents = jet.constituents();

//     // find the hardest prong
//     auto hardest =
//         *std::max_element(constituents.begin(), constituents.end(),
//                           [](const PseudoJet& c1, const PseudoJet& c2) {
//                               return c1.pt() < c2.pt();
//                           });
//     return false;
// }

JetLevelData reconstructObjects(const HadronLevelData& hadrons) {
    JetLevelData objs;

    objs.photons = isolatedObjects(hadrons.photons);
    objs.electrons = isolatedObjects(hadrons.electrons);
    objs.muons = isolatedObjects(hadrons.muons);

    auto clusteredJets = getJets(hadrons);
    for (auto& j : clusteredJets) j.set_user_index(0);

    auto btagged = bTag(hadrons.bPartons, clusteredJets, 0.5);
    objs.bJets = btagged.first;
    // auto normalJets = btagged.second;

    return objs;
}
