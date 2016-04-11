/*
 *  p p --> b1 + b2 --> h s + b2
 */
#include "cascade1.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "CLHEF/lhef.h"
#include "Pythia8/Pythia.h"
#include "TGenPhaseSpace.h"
#include "TLorentzVector.h"

using std::vector;

const double PROD[2] = {0.5, 0.5};

int main(int argc, char* argv[]) {
    std::string appname = "cascade1";
    if (argc != 3) {
        std::cerr << " Usage: " << appname << " output nevent\n"
                  << " output - output file\n"
                  << " nevent - number of events\n"
                  << " ex) " << appname << " test.lhe 100 > test.log\n";
        return 1;
    }

    // Generator.
    Pythia8::Pythia pythia;
    pythia.readFile("cascade1.cmnd");
    pythia.init();

    // Output file.
    ofstream outfile;
    outfile.open(argv[1]);
    outfile << lhef::openingLine() << '\n';
    lhef::GlobalInfo info(pythia.info.idA(), pythia.info.idB(),
                          pythia.info.eA(), pythia.info.eB(), 0, 0, 3, 0, 0, 1,
                          vector<double>({0.0}), vector<double>({0.0}),
                          vector<double>({1.0}), vector<int>({1}));
    outfile << info << '\n';

    const int nevent = std::atoi(argv[2]);
    TGenPhaseSpace bDecay;
    // Loop over events.
    for (int ieve = 0; ieve != nevent; ++ieve) {
        if (!pythia.next()) continue;

        lhef::Particles ps;
        std::vector<TLorentzVector> bmesons;
        for (int ip = 0; ip != pythia.event.size(); ++ip) {
            auto p = pythia.event.at(ip);
            if (p.status() == -21) {  // incoming particles.
                ps.push_back(toLHEFParticle(-1, 0, 0, p));
            } else if (p.isFinal() && isBMeson(p)) {  // B meson.
                ps.push_back(toLHEFParticle(2, 1, 2, p));
                bmesons.push_back(particleMomentum(p));
            }
        }

        if (!bmesons.empty()) {
            bDecay.SetDecay(bmesons[0], 2, PROD);
            double weight = bDecay.Generate();
            auto h2 = toParticle(35, *(bDecay.GetDecay(0)));
            ps.push_back(toLHEFParticle(1, 3, 3, h2));
            auto s = toParticle(3, *(bDecay.GetDecay(1)));
            ps.push_back(toLHEFParticle(1, 3, 3, s));
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
    }  // event loop.

    outfile << lhef::closingLine();
    outfile.close();

    // Statistics: full printout.
    pythia.stat();
}
