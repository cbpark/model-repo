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

    std::vector<Pythia8::Particle> allParticlesExcept(int i) const {
        std::vector<Pythia8::Particle> ps;
        for (const auto& p : photons) {
            if (p.first == i) continue;
            ps.push_back(p.second);
        }
        for (const auto& p : muons) {
            if (p.first == i) continue;
            ps.push_back(p.second);
        }
        for (const auto& p : others) {
            ps.push_back(p);
        }
        return ps;
    }
};

std::pair<HadronLevelData, double> getHadronLevelData(const Pythia8::Event& ev);

#endif  // MODELS_PYTHIA8_SM_QQBAR_ZH_HADRON_LEVEL_DATA_H_
