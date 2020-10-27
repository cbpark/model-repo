#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <utility>

#include "TGenPhaseSpace.h"
#include "TLorentzVector.h"

#include "lhef/lhef.h"
#include "lhef/particle.h"

const double EPS = 1.0e-20;

const double MTAU = 1.777;
const double MMU = 0.105;

std::pair<int, int> decay_chain();

int main(int argc, char *argv[]) {
    std::string appname = "ditau_4";
    if (argc != 3) {
        std::cerr << " Usage: " << appname << " output nevent\n"
                  << " output - the output file\n"
                  << " nevent - the number of events\n"
                  << " ex) " << appname << " event.lhe 100\n";
        return 1;
    }

    std::ofstream outfile(argv[1]);
    outfile << lhef::openingLine() << '\n';
    outfile << "<init>\n  11  -11  7.0  4.0  0 0 0 0 0 0\n  0 0 0 0\n</init>\n";

    TLorentzVector em{0.0, 0.0, 7.0, 7.0};
    TLorentzVector ep{0.0, 0.0, -4.0, 4.0};
    TLorentzVector s = em + ep;

    const double MTAU2[2] = {MTAU, MTAU};
    const double MMuNuNu[3] = {MMU, EPS, EPS};

    TGenPhaseSpace event;
    event.SetDecay(s, 2, MTAU2);  // --> tau + tau

    int nevent = std::atoi(argv[2]);
    for (auto iev = 0; event.Generate(), iev < nevent; ++iev) {
        lhef::Particles ps;
        // line 1
        ps.push_back(lhef::Particle(11, -1, 0, 0, 0, 0, em.Px(), em.Py(),
                                    em.Pz(), em.E(), em.M(), 0, 0));
        // line 2
        ps.push_back(lhef::Particle(-11, -1, 0, 0, 0, 0, ep.Px(), ep.Py(),
                                    ep.Pz(), ep.E(), ep.M(), 0, 0));

        const auto chain = decay_chain();
        auto tau1 = event.GetDecay(chain.first);
        // line 3
        ps.push_back(lhef::Particle(15, 2, 1, 2, 0, 0, tau1->Px(), tau1->Py(),
                                    tau1->Pz(), tau1->E(), tau1->M(), 0, 0));
        auto tau2 = event.GetDecay(chain.second);
        // line 4
        ps.push_back(lhef::Particle(-15, 2, 1, 2, 0, 0, tau2->Px(), tau2->Py(),
                                    tau2->Pz(), tau2->E(), tau2->M(), 0, 0));

        TGenPhaseSpace tau1_decay, tau2_decay;
        tau1_decay.SetDecay(*tau1, 3, MMuNuNu);  // --> mu + nu + nu
        tau2_decay.SetDecay(*tau2, 3, MMuNuNu);  // --> mu + nu + nu

        tau1_decay.Generate();
        auto mu1 = tau1_decay.GetDecay(0);
        // line 5
        ps.push_back(lhef::Particle(13, 1, 3, 3, 0, 0, mu1->Px(), mu1->Py(),
                                    mu1->Pz(), mu1->E(), mu1->M(), 0, 0));
        auto nu1 = tau1_decay.GetDecay(1);
        // line 6
        ps.push_back(lhef::Particle(-14, 1, 3, 3, 0, 0, nu1->Px(), nu1->Py(),
                                    nu1->Pz(), nu1->E(), nu1->M(), 0, 0));
        auto nu2 = tau1_decay.GetDecay(2);
        // line 7
        ps.push_back(lhef::Particle(16, 1, 3, 3, 0, 0, nu2->Px(), nu2->Py(),
                                    nu2->Pz(), nu2->E(), nu2->M(), 0, 0));

        tau2_decay.Generate();
        auto mu2 = tau2_decay.GetDecay(0);
        // line 8
        ps.push_back(lhef::Particle(-13, 1, 4, 4, 0, 0, mu2->Px(), mu2->Py(),
                                    mu2->Pz(), mu2->E(), mu2->M(), 0, 0));
        auto nu3 = tau2_decay.GetDecay(1);
        // line 9
        ps.push_back(lhef::Particle(14, 1, 4, 4, 0, 0, nu3->Px(), nu3->Py(),
                                    nu3->Pz(), nu3->E(), nu3->M(), 0, 0));
        auto nu4 = tau2_decay.GetDecay(2);
        // line 10
        ps.push_back(lhef::Particle(-16, 1, 4, 4, 0, 0, nu4->Px(), nu4->Py(),
                                    nu4->Pz(), nu4->E(), nu4->M(), 0, 0));

        lhef::EventEntry entry;
        int i = 1;
        for (const auto &p : ps) {
            entry.insert({i, p});
            ++i;
        }
        lhef::EventInfo evinfo(10, 9999, 0, 0, 0, 0);
        lhef::Event ev(evinfo, entry);
        outfile << ev << '\n';
    }

    outfile << lhef::closingLine();
    outfile.close();
}

std::pair<int, int> decay_chain() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dr(0.0, 1.0);
    int chain1 = dr(gen) < 0.5 ? 0 : 1;
    int chain2 = 1 - chain1;
    return {chain1, chain2};
}
