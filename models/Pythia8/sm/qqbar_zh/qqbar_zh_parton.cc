#include "qqbar_zh_parton.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Pythia8/Pythia.h"
#include "CLHEF/lhef.h"

using std::vector;

int main(int argc, char* argv[]) {
    std::string appname = "qqbar_zh_parton";
    if (argc != 3) {
        std::cerr << " Usage: " << appname << " output nevent\n"
                  << " output - output file\n"
                  << " nevent - number of events\n"
                  << " ex) " << appname << " test.lhe 100 > test.log\n";
        return 1;
    }

    // Generator.
    Pythia8::Pythia pythia;
    pythia.readFile("qqbar_zh_parton.cmnd");
    pythia.init();

    ofstream outfile;
    outfile.open(argv[1]);
    outfile << lhef::openingLine() << '\n';
    lhef::GlobalInfo info(pythia.info.idA(), pythia.info.idB(),
                          pythia.info.eA(), pythia.info.eB(), 0, 0, 3, 0, 0, 1,
                          vector<double>({0.0}), vector<double>({0.0}),
                          vector<double>({1.0}), vector<int>({1}));
    outfile << info << '\n';

    int nevent = std::atoi(argv[2]);
    // Loop over events.
    for (int ieve = 0; ieve != nevent; ++ieve) {
        if (!pythia.next()) continue;

        lhef::Particles ps;
        for (int ip = 0; ip != pythia.event.size(); ++ip) {
            auto p = pythia.event.at(ip);

            if (p.statusAbs() == 21) {
                // incoming particles
                ps.push_back(toLHEFParticle(-1, 0, 0, p));
            } else if (p.statusAbs() == 22 && p.id() == 25) {
                // Higgs boson
                ps.push_back(toLHEFParticle(2, 1, 2, p));
            } else if (p.statusAbs() == 22 && p.id() == 23) {
                // Z boson
                ps.push_back(toLHEFParticle(2, 1, 2, p));
            } else if (p.statusAbs() == 23 && p.idAbs() == 15 &&
                       pythia.event.at(p.mother1()).id() == 25) {
                // tau leptons from Higgs
                ps.push_back(toLHEFParticle(2, 3, 3, p));

                // daughters of the tau lepton
                // - This may contain pi0, which will eventually decay
                //   into photons. Hence, pi0 can be considered as a
                //   visible particle.
                vector<int> daughters = p.daughterList();
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
