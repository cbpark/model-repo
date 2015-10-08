#include "gg_h_tautau_parton.h"
#include "Pythia8/Pythia.h"
#include "CLHEF/lhef.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool fromHiggs(const Pythia8::Particle& p, const Pythia8::Event ev) {
    std::vector<int> mothers = p.motherList();
    for (const auto& m : mothers) {
        if (ev[m].idAbs() == 25) return true;
    }
    return false;
}

Pythia8::Particle finalTau(const Pythia8::Particle& p,
                           const Pythia8::Event& ev) {
    auto daughter = ev[p.daughter1()];
    if (daughter.idAbs() == 15)
        return finalTau(daughter, ev);
    else
        return p;
}

int main(int argc, char* argv[]) {
    std::string appname = "gg_h_tautau_parton_isr";
    if (argc != 3) {
        std::cerr << " Usage: " << appname << " output nevent\n"
                  << " output - output file\n"
                  << " nevent - number of events\n"
                  << " ex) " << appname << " test.lhe 100 > test.log\n";
        return 1;
    }

    // Generator.
    Pythia8::Pythia pythia;
    pythia.readFile("gg_h_tautau_parton_isr.cmnd");
    pythia.init();

    ofstream outfile;
    outfile.open(argv[1]);
    outfile << lhef::openingLine() << '\n';
    lhef::GlobalInfo info(pythia.info.idA(), pythia.info.idB(),
                          pythia.info.eA(), pythia.info.eB(), 0, 0, 3, 0, 0, 1,
                          std::vector<double>({0.0}),
                          std::vector<double>({0.0}),
                          std::vector<double>({1.0}), std::vector<int>({1}));
    outfile << info << '\n';

    int nevent = std::atoi(argv[2]);
    // Loop over events.
    for (int ieve = 0; ieve != nevent; ++ieve) {
        if (!pythia.next()) continue;

        auto event = pythia.event;
        lhef::Particles ps;
        for (int ip = 0; ip != event.size(); ++ip) {
            auto p = event[ip];

            if (p.statusAbs() == 21) {
                // incoming particles
                ps.push_back(toLHEFParticle(-1, 0, 0, p));
            } else if (p.statusAbs() == 62 && p.id() == 25) {
                // Higgs boson
                ps.push_back(toLHEFParticle(2, 1, 2, p));
            } else if (p.idAbs() == 15 && fromHiggs(p, event)) {
                p = finalTau(p, event);

                // tau leptons from Higgs
                ps.push_back(toLHEFParticle(2, 3, 3, p));

                // daughters of the tau lepton
                // - This may contain pi0, which will eventually decay
                //   into photons. Hence, pi0 can be considered as a
                //   visible particle.
                std::vector<int> daughters = p.daughterList();
                for (const auto& d : daughters) {
                    auto daughter = pythia.event.at(d);
                    int motherline = daughter.mother1();
                    ps.push_back(
                        toLHEFParticle(1, motherline, motherline, daughter));
                }
            }
        }

        lhef::EventEntry entry;
        int i = 1;
        for (const auto& p : ps) {
            entry.insert({i, p});
            ++i;
        }

        lhef::EventInfo evinfo(ps.size(), 0, pythia.info.weight(),
                               pythia.info.scalup(), pythia.info.alphaEM(),
                               pythia.info.alphaS());
        lhef::Event ev(evinfo, entry);
        outfile << ev << '\n';
    }  // event loop

    outfile << lhef::closingLine();
    outfile.close();

    // Statistics: full printout.
    pythia.stat();
}
