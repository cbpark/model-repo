#include "jet_level_data.h"
#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <utility>
#include <vector>
#include "Pythia8/Pythia.h"
#include "hadron_level_data.h"

using Pythia8::Particle;
using fastjet::PseudoJet;
using fastjet::ClusterSequence;
using PseudoJets = std::vector<PseudoJet>;

PseudoJet py2FjInput(const Particle& p) {
    PseudoJet fjInput(p.px(), p.py(), p.pz(), p.e());
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

std::pair<PseudoJets, std::unique_ptr<ClusterSequence>> jetObjects(
    const HadronLevelData& hadrons) {
    PseudoJets particles;
    for (const auto& h : hadrons.others) particles.push_back(py2FjInput(h));

    fastjet::JetDefinition jetDef(fastjet::antikt_algorithm, 0.5,
                                  fastjet::E_scheme, fastjet::Best);
    auto clusterSeq = std::unique_ptr<ClusterSequence>(
        new ClusterSequence(particles, jetDef));

    // Central hard jets.
    PseudoJets jets = clusterSeq->inclusive_jets(20.0);
    jets.erase(std::remove_if(jets.begin(), jets.end(), [](const PseudoJet& j) {
                   return std::fabs(j.eta()) > 2.5;
               }), jets.end());
    return std::make_pair(jets, std::move(clusterSeq));
}

void bTagging(PseudoJets* jets, const std::vector<Particle>& bquarks,
              double coneR) {
    if (!jets->empty()) {
        for (const auto& b : bquarks) {
            if (b.pT() < 20.0 || fabs(b.eta()) > 2.5) continue;

            PseudoJet bquark(b.px(), b.py(), b.pz(), b.e());
            auto j = std::min_element(
                jets->begin(), jets->end(),
                [&bquark](const PseudoJet& j1, const PseudoJet& j2) {
                    return j1.delta_R(bquark) < j2.delta_R(bquark);
                });
            if (j->delta_R(bquark) < coneR) j->set_user_index(5);
        }
    }
}

int charge(const PseudoJet& j) {
    int id = j.user_index();
    if (id == -11 || id == -13 || id == 211 || id == 321 || id == 2212)
        return 1;
    else if (id == 11 || id == 13 || id == -211 || id == -321 || id == -2212)
        return -1;
    else
        return 0;
}

PseudoJets filterConstituents(const PseudoJets& constituents,
                              std::function<bool(const PseudoJet&)> pred) {
    PseudoJets filtered;
    std::copy_if(constituents.cbegin(), constituents.cend(),
                 std::back_inserter(filtered), pred);
    return filtered;
}

bool isTauJet(const PseudoJet& jet, const ClusterSequence& clusterSeq,
              double pTthres) {
    if (jet.pt() < pTthres) return false;

    PseudoJets constituents = clusterSeq.constituents(jet);
    PseudoJets charged = filterConstituents(
        constituents, [](const PseudoJet& c) { return charge(c) != 0; });
    if (charged.empty()) return false;

    // The seed is the hardest charged particle in the jet.
    auto hardest =
        *std::max_element(charged.cbegin(), charged.cend(),
                          [](const PseudoJet& c1, const PseudoJet& c2) {
                              return c1.pt() < c2.pt();
                          });

    // Seed must be hard enough and close to the jet vector.
    if (hardest.pt() < 5.0 || hardest.delta_R(jet) > 0.1) return false;

    // No charged hard tracks near the seed.
    PseudoJets chargedNear =
        filterConstituents(charged, [hardest](const PseudoJet& c) {
            if (c.pt() < 1.0) return false;
            double dR = hardest.delta_R(c);
            return dR > 0.07 && dR < 0.4;
        });
    if (!chargedNear.empty()) return false;

    // No hard photons near the seed.
    PseudoJets photonNear =
        filterConstituents(constituents, [hardest](const PseudoJet& c) {
            if (c.user_index() != 22 || c.pt() < 1.5) return false;
            double dR = hardest.delta_R(c);
            return dR > 0.07 && dR < 0.4;
        });
    if (!photonNear.empty()) return false;

    return true;
}

JetLevelData reconstructObjects(const HadronLevelData& hadrons) {
    JetLevelData objs;

    objs.photons = isolatedObjects(hadrons.photons);
    objs.electrons = isolatedObjects(hadrons.electrons);
    objs.muons = isolatedObjects(hadrons.muons);

    auto jetobjs = jetObjects(hadrons);
    auto jets = jetobjs.first;
    for (auto& j : jets) j.set_user_index(0);

    // b tagging.
    bTagging(&jets, hadrons.bPartons, 0.5);

    PseudoJets normalJets;
    std::partition_copy(jets.cbegin(), jets.cend(),
                        std::back_inserter(objs.bJets),
                        std::back_inserter(normalJets),
                        [](const PseudoJet& j) { return j.user_index() == 5; });
    fastjet::sorted_by_pt(objs.bJets);

    auto clusterSeq = std::move(jetobjs.second);
    std::partition_copy(
        normalJets.cbegin(), normalJets.cend(), std::back_inserter(objs.taus),
        std::back_inserter(objs.jets), [&clusterSeq](const PseudoJet& j) {
            return isTauJet(j, *clusterSeq, 20.0);
        });
    fastjet::sorted_by_pt(objs.jets);
    fastjet::sorted_by_pt(objs.taus);

    return objs;
}
