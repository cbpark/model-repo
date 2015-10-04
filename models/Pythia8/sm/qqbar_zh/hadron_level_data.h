#ifndef MODELS_PYTHIA8_SM_QQBAR_ZH_HADRON_LEVEL_DATA_H_
#define MODELS_PYTHIA8_SM_QQBAR_ZH_HADRON_LEVEL_DATA_H_

#include <map>
#include <utility>
#include <vector>
#include "Pythia8/Pythia.h"

struct HadronLevelData {
    std::vector<Pythia8::Particle> bPartons;
    std::vector<Pythia8::Particle> tauLeptons;

    std::map<int, Pythia8::Particle> photons;
    std::map<int, Pythia8::Particle> electrons;
    std::map<int, Pythia8::Particle> muons;
    std::vector<Pythia8::Particle> others;
    // pT and phi of the missing energy vector.
    std::pair<double, double> met;

    std::vector<Pythia8::Particle> allParticlesExcept(int i) const {
        std::vector<Pythia8::Particle> ps;
        auto insert_ = [&ps, i](const std::map<int, Pythia8::Particle>& ls) {
            for (const auto& l : ls) {
                if (l.first == i) continue;
                ps.push_back(l.second);
            }
        };

        insert_(photons);
        insert_(electrons);
        insert_(muons);
        ps.insert(ps.end(), others.begin(), others.end());
        return ps;
    }
};

HadronLevelData getHadronLevelData(const Pythia8::Event& ev);

#endif  // MODELS_PYTHIA8_SM_QQBAR_ZH_HADRON_LEVEL_DATA_H_
