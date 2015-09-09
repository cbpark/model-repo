#include "gg_h_tautau_parton.h"
#include "Pythia8/Pythia.h"
#include "CLHEF/lhef.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

void PythiaSettings(Pythia8::Pythia *pythia) {
    // pythia->readString("PartonLevel:all = off");
    pythia->readString("PartonLevel:FSR = off");
    pythia->readString("PartonLevel:ISR = off");
    pythia->readString("PartonLevel:MPI = off");

    // LHC 14 TeV initialization.
    pythia->readString("Beams:eCM = 14000.0");

    // Process selection.
    pythia->readString("HiggsSM:gg2H = on");

    // parameters
    pythia->readString("25:m0 = 125.0");
    pythia->readString("25:onMode = off");
    pythia->readString("25:onIfAll = 15 15");

    // Tau decays
    pythia->readString("TauDecays:mode = 2");
    pythia->readString("TauDecays:tauPolarization = 0");
    pythia->readString("TauDecays:tauMother = 25");
    pythia->readString("15:9:meMode = 1543");
}

int main(int argc, char* argv[]) {
    std::string appname = "gg_h_tautau_parton";
    if (argc != 3) {
        std::cerr << " Usage: " << appname << " output nevent\n"
                  << " output - output file\n"
                  << " nevent - number of events\n"
                  << " ex) " << appname << " test.lhe 100 > test.log\n";
        return 1;
    }

    // Generator.
    Pythia8::Pythia pythia;
    PythiaSettings(&pythia);
    pythia.init();

    ofstream outfile;
    outfile.open(argv[1]);
    outfile << lhef::OpeningLine() << '\n';
    lhef::GlobalInfo info(pythia.info.idA(), pythia.info.idB(),
                          pythia.info.eA(), pythia.info.eB(),
                          0, 0, 3, 0, 0, 1,
                          std::vector<double>({0.0}),
                          std::vector<double>({0.0}),
                          std::vector<double>({1.0}), std::vector<int>({1}));
    outfile << info << '\n';

    int nevent = std::atoi(argv[2]);
    // Loop over events.
    for (int ieve = 0; ieve < nevent; ++ieve) {
        if (!pythia.next()) {
            continue;
        }

        lhef::Particles ps;
        for (int i = 0; i < pythia.event.size(); ++i) {
            auto p = pythia.event.at(i);

            if (p.statusAbs() == 61) {
                // incoming particles
                ps.push_back(ToLHEFParticle(-1, 0, 0, p));
            } else if (p.statusAbs() == 62 && p.id() == 25) {
                // Higgs boson
                ps.push_back(ToLHEFParticle(2, 1, 2, p));
            } else if (p.statusAbs() == 23 && p.idAbs() == 15
                       && pythia.event.at(p.mother1()).id() == 25) {
                // tau leptons from Higgs
                ps.push_back(ToLHEFParticle(2, 3, 3, p));
            } else if (p.statusAbs() == 91
                       && pythia.event.at(p.mother1()).idAbs() == 15) {
                // daughters of tau lepton
                // - This may contain pi0, which will eventually decay
                //   into photons. Hence, pi0 can be considered as a
                //   visible particle.
                int motherline = p.mother1() - 9;
                ps.push_back(ToLHEFParticle(1, motherline, motherline, p));
            }
        }

        lhef::EventEntry entry;
        int i = 1;
        for (const auto& p : ps) {
            entry.insert({i, p});
            ++i;
        }

        lhef::EventInfo evinfo(ps.size(), 0,
                               pythia.info.weight(), pythia.info.scalup(),
                               pythia.info.alphaEM(), pythia.info.alphaS());
        lhef::Event ev(evinfo, entry);
        outfile << ev << '\n';
    }  // event loop

    outfile << lhef::ClosingLine();
    outfile.close();

    // Statistics: full printout.
    pythia.stat();
}
