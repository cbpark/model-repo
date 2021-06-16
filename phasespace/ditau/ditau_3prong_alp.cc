/* Copyright 2021 Chan Beom Park <cbpark@gmail.com> */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <utility>

#include "TGenPhaseSpace.h"
#include "TLorentzVector.h"
#include "TRandom.h"

#include "lhef/lhef.h"
#include "lhef/particle.h"

using std::cout;

const char APPNAME[] = "ditau_3prong_alp";

const double MTAU = 1.777;
const double MELEC = 5.11e-4;
const double MPION = 0.13957;

std::pair<int, int> decay_chain();

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << APPNAME << " output malp nevent\n"
                  << "  output - the output file\n"
                  << "  malp - ALP mass in GeV\n"
                  << "  nevent - the number of events\n"
                  << "  ex) " << APPNAME << " event.lhe 1.0 100\n";
        return 1;
    }

    std::ofstream outfile(argv[1]);
    outfile << lhef::openingLine() << '\n';
    outfile << "<init>\n11 -11 7.000000e+00 4.000000e+00 0 0 247000 247000 -4 "
               "1\n0 0 0 1\n</init>\n";

    TLorentzVector em{0.0, 0.0, 6.9999999813485, 7.0};
    TLorentzVector ep{0.0, 0.0, -3.9999999673598747, 4.0};
    TLorentzVector s = em + ep;
    const double MTAU2[2] = {MTAU, MTAU};

    const double malp = std::atof(argv[2]);
    cout << APPNAME << ": mALP = " << malp << " GeV\n";
    const double MElecAlp[2] = {MELEC, malp};
    const double M3PiNu[4] = {MPION, MPION, MPION, 0.0};

    TGenPhaseSpace event;
    event.SetDecay(s, 2, MTAU2);  // --> tau + tau

    int nevent = std::atoi(argv[3]);
    cout << APPNAME << ": we will generate " << nevent
         << " event for 3x1 process.\n";
    gRandom = new TRandom(0);
    for (auto iev = 1; iev != nevent + 1; ++iev) {
        event.Generate();

        lhef::Particles ps;
        // line 1
        ps.push_back(lhef::Particle(11, -1, 0, 0, 0, 0, em.Px(), em.Py(),
                                    em.Pz(), em.E(), em.M(), 0, 1));
        // line 2
        ps.push_back(lhef::Particle(-11, -1, 0, 0, 0, 0, ep.Px(), ep.Py(),
                                    ep.Pz(), ep.E(), ep.M(), 0, -1));

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
        tau1_decay.SetDecay(*tau1, 2, MElecAlp);  // --> e + a
        tau2_decay.SetDecay(*tau2, 4, M3PiNu);    // --> 3 pi + nu

        tau1_decay.Generate();
        auto elec = tau1_decay.GetDecay(0);
        // line 5
        ps.push_back(lhef::Particle(11, 1, 3, 3, 0, 0, elec->Px(), elec->Py(),
                                    elec->Pz(), elec->E(), elec->M(), 0, 0));
        auto alp = tau1_decay.GetDecay(1);
        // line 6
        ps.push_back(lhef::Particle(40, 1, 3, 3, 0, 0, alp->Px(), alp->Py(),
                                    alp->Pz(), alp->E(), alp->M(), 0, 0));

        tau2_decay.Generate();
        auto pion = tau2_decay.GetDecay(0);
        // line 7
        ps.push_back(lhef::Particle(-211, 1, 4, 4, 0, 0, pion->Px(), pion->Py(),
                                    pion->Pz(), pion->E(), pion->M(), 0, 0));
        pion = tau2_decay.GetDecay(1);
        // line 8
        ps.push_back(lhef::Particle(211, 1, 4, 4, 0, 0, pion->Px(), pion->Py(),
                                    pion->Pz(), pion->E(), pion->M(), 0, 0));
        pion = tau2_decay.GetDecay(2);
        // line 9
        ps.push_back(lhef::Particle(211, 1, 4, 4, 0, 0, pion->Px(), pion->Py(),
                                    pion->Pz(), pion->E(), pion->M(), 0, 0));
        auto nu = tau2_decay.GetDecay(3);
        // line 10
        ps.push_back(lhef::Particle(-16, 1, 4, 4, 0, 0, nu->Px(), nu->Py(),
                                    nu->Pz(), nu->E(), nu->M(), 0, 0));

        lhef::EventEntry entry;
        int i = 1;
        for (const auto &p : ps) {
            entry.insert({i, p});
            ++i;
        }
        lhef::EventInfo evinfo(10, 1, +4.0871796e-02, 1.05830100e+01,
                               7.81860800e-03, 1.76468800e-01);
        lhef::Event ev(evinfo, entry);
        outfile << ev << '\n';

        if ((iev < 10000 && iev % 1000 == 0) || iev % 10000 == 0) {
            cout << "... processed " << iev << " events.\n";
        }
    }

    outfile << lhef::closingLine();
    outfile.close();
    cout << APPNAME << ": the events have been stored in " << argv[1] << '\n';
    cout << APPNAME << ": done!\n";
}

std::pair<int, int> decay_chain() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dr(0.0, 1.0);
    int chain1 = dr(gen) < 0.5 ? 0 : 1;
    int chain2 = 1 - chain1;
    return {chain1, chain2};
}
