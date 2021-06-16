/* Copyright 2021 Chan Beom Park <cbpark@gmail.com> */

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "TGenPhaseSpace.h"
#include "TLorentzVector.h"
#include "TRandom3.h"

#include "lhef/lhef.h"

using std::cout;

const char APPNAME[] = "ditau_alp_decay";

const double MELEC = 5.11e-4;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << APPNAME << " input output malp\n"
                  << "  input  - the input LHEF file\n"
                  << "  output - the output file\n"
                  << "  malp - ALP mass in GeV\n"
                  << "  ex) " << APPNAME << " event.lhe decay.lhe 1.0\n";
        return 1;
    }

    std::ifstream lhe_input{argv[1]};
    if (!lhe_input) {
        std::cerr << APPNAME << ": failed to open " << argv[1] << '\n';
        return 1;
    } else {
        cout << APPNAME << ": the input file is " << argv[1] << '\n';
    }

    std::ofstream outfile{argv[2]};
    outfile << lhef::openingLine() << '\n';
    outfile << "<init>\n11 -11 7.000000e+00 4.000000e+00 0 0 247000 247000 -4 "
               "2\n0 0 0 1\n0 0 0 1\n</init>\n";

    const double malp = std::atof(argv[3]);
    cout << APPNAME << ": mALP = " << malp << " GeV\n";
    TGenPhaseSpace tau;
    const double MElecAlp[2] = {MELEC, malp};

    auto lhe = lhef::parseEvent(&lhe_input);
    gRandom = new TRandom3(0);
    for (auto iev = 1; !lhe.empty();
         lhe = lhef::parseEvent(&lhe_input), ++iev) {
        auto entries = lhe.particleEntries();

        // is there a tau in the final state?
        auto undecayed_tau_pos =
            std::find_if(entries.begin(), entries.end(), [](const auto &p) {
                return std::abs(p.second.pid()) == 15 && p.second.status() == 1;
            });
        if (undecayed_tau_pos == entries.end()) { continue; }

        auto decayed_tau = undecayed_tau_pos->second;
        decayed_tau.set_status(2);  // set it to be unstable
        undecayed_tau_pos->second = decayed_tau;

        TLorentzVector tau_momentum{decayed_tau.px(), decayed_tau.py(),
                                    decayed_tau.pz(), decayed_tau.energy()};
        tau.SetDecay(tau_momentum, 2, MElecAlp);

        tau.Generate();

        auto tau_line_num = undecayed_tau_pos->first auto decay_prod =
            tau.GetDecay(0);  // electron
        auto elec =
            lhef::Particle(11 * std::abs(decayed_tau.pid()) / decayed_tau.pid(),
                           1, tau_line_num, tau_line_num, 0, 0,
                           decay_prod->Px(), decay_prod->Py(), decay_prod->Pz(),
                           decay_prod->E(), decay_prod->M(), 0, 0);
        entries.insert({9, elec});
        decay_prod = tau.GetDecay(1);  // ALP
        auto alp =
            lhef::Particle(40, 1, tau_line_num, tau_line_num, 0, 0,
                           decay_prod->Px(), decay_prod->Py(), decay_prod->Pz(),
                           decay_prod->E(), decay_prod->M(), 0, 0);
        entries.insert({10, alp});

        // copy the event information.
        auto evinfo0 = lhe.eventInfo();
        // two more lines added!
        lhef::EventInfo evinfo(evinfo0.nup + 2, evinfo0.idprup, evinfo0.xwgtup,
                               evinfo0.scalup, evinfo0.aqedup, evinfo0.aqcdup);
        lhef::Event ev(evinfo, entries);

        // write the decayed event.
        outfile << ev << '\n';

        if ((iev < 10000 && iev % 1000 == 0) || iev % 10000 == 0) {
            cout << "... processed " << iev << " events.\n";
        }
    }

    outfile << lhef::closingLine();
    outfile.close();
    cout << APPNAME << ": the events have been stored in " << argv[2] << '\n';
    cout << APPNAME << ": done!\n";
}
