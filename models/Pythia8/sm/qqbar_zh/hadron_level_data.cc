#include "hadron_level_data.h"

#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

#include "Pythia8/Pythia.h"

using Pythia8::Event;
using Pythia8::Particle;

bool isNeutrino(const Particle& p) {
    auto id = p.idAbs();
    return id == 12 || id == 14 || id == 16;
}

bool isHardCentral(const Particle& p, double ptMin, double etaMax) {
    return p.pT() > ptMin && std::fabs(p.eta()) < etaMax;
}

bool isolated(const std::pair<int, Particle>& p, const HadronLevelData& f,
              double isoR, double isoFrac) {
    std::vector<Particle> allOthers = f.allParticlesExcept(p.first);
    double sumPtInCone = 0.0;
    auto candidate = p.second;
    for (const auto& other : allOthers) {
        if (Pythia8::REtaPhi(candidate.p(), other.p()) < isoR)
            sumPtInCone += other.pT();
    }
    return sumPtInCone / candidate.pT() < isoFrac ? true : false;
}

void restoreNonIsolated(HadronLevelData* f) {
    auto restoreNonIsolated_ = [&f](std::map<int, Pythia8::Particle>* ps,
                                    double isoR, double isoFrac) {
        for (auto p = ps->begin(); p != ps->end();) {
            if (!isolated(*p, *f, isoR, isoFrac)) {
                f->others.push_back(p->second);
                ps->erase(p++);
            } else {
                ++p;
            }
        }
    };

    auto photons = f->photons;
    restoreNonIsolated_(&photons, 0.3, 0.1);
    f->photons = photons;

    auto electrons = f->electrons;
    restoreNonIsolated_(&electrons, 0.3, 0.1);
    f->electrons = electrons;

    auto muons = f->muons;
    restoreNonIsolated_(&muons, 0.3, 0.1);
    f->muons = muons;
}

std::pair<HadronLevelData, double> getHadronLevelData(const Event& ev) {
    HadronLevelData hadronLevel;
    Pythia8::Vec4 missingETvec(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i != ev.size(); ++i) {
        auto particle = ev[i];
        auto pid = particle.idAbs();

        if (particle.status() == -23) {
            if (pid == 5 && ev[particle.mother1()].idAbs() == 23) {
                // b quarks from the Z boson
                hadronLevel.bPartons.push_back(particle);
            } else if (pid == 15 && ev[particle.mother1()].idAbs() == 25) {
                // taus from the Higgs boson
                hadronLevel.tauLeptons.push_back(particle);
            }
        }

        // From now on, final states only
        if (!particle.isFinal()) continue;
        // No neutrinos
        if (isNeutrino(particle)) continue;
        // |eta| < 5.0 and hard enough
        if (std::fabs(particle.eta()) > 5.0 || particle.pT() < 0.1) continue;

        // Missing ET
        missingETvec -= particle.p();

        if (pid == 22 && isHardCentral(particle, 20.0, 2.5)) {
            hadronLevel.photons.emplace(i, particle);
            continue;
        } else if (pid == 11 && isHardCentral(particle, 20.0, 2.5)) {
            hadronLevel.electrons.emplace(i, particle);
            continue;
        } else if (pid == 13 && isHardCentral(particle, 20.0, 2.5)) {
            hadronLevel.muons.emplace(i, particle);
            continue;
        }

        hadronLevel.others.push_back(particle);
    }
    restoreNonIsolated(&hadronLevel);

    return std::make_pair(hadronLevel, missingETvec.pT());
}