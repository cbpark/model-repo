#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "TGenPhaseSpace.h"
#include "TLorentzVector.h"
#include "TRandom3.h"

#include "lhef/lhef.h"
#include "lhef/particle.h"

const double MH = 800.0;

double sign_num(TRandom &r) {
    double x = r.Rndm();
    return x > 0.5 ? 1 : -1;
}

int main(int argc, char *argv[]) {
    std::string appname = "antler_isr";
    if (argc != 3) {
        std::cerr << " Usage: " << appname << " output nevent\n"
                  << " output - the output file\n"
                  << " nevent - the number of events\n"
                  << " ex) " << appname << " event.lhe 100\n";
        return 1;
    }

    std::ofstream outfile(argv[1]);
    outfile << lhef::openingLine() << '\n';

    auto r = TRandom3(42);

    double mT[2] = {173.0, 173.0};
    double mWV[2] = {80.379, 4.8};

    int nevent = std::atoi(argv[2]);
    for (auto iev = 0; iev < nevent; ++iev) {
        double pt_isr = 1000 * r.Rndm();
        double cosphi = -1 + 2 * r.Rndm();
        double sinphi = std::sqrt(1 - cosphi * cosphi);
        double sgn = sign_num(r);
        double pz_isr = sgn * 1000 * r.Rndm();

        TLorentzVector H(pt_isr * cosphi, pt_isr * sinphi, pz_isr,
                         std::sqrt(MH * MH + pt_isr * pt_isr));
        TGenPhaseSpace hdecay;
        hdecay.SetDecay(H, 2, mT);
        hdecay.Generate();

        lhef::Particles ps;
        ps.push_back(lhef::Particle(35, 2, 0, 0, 0, 0, H.Px(), H.Py(), H.Pz(),
                                    H.E(), H.M(), 0, 0));

        auto pT1 = hdecay.GetDecay(0);
        ps.push_back(lhef::Particle(6, 2, 1, 0, 0, 0, pT1->Px(), pT1->Py(),
                                    pT1->Pz(), pT1->E(), pT1->M(), 0, 0));
        auto pT2 = hdecay.GetDecay(1);
        ps.push_back(lhef::Particle(-6, 2, 1, 0, 0, 0, pT2->Px(), pT2->Py(),
                                    pT2->Pz(), pT2->E(), pT2->M(), 0, 0));

        TGenPhaseSpace t1decay, t2decay;
        t1decay.SetDecay(*pT1, 2, mWV);
        t2decay.SetDecay(*pT2, 2, mWV);

        t1decay.Generate();
        auto pW1 = t1decay.GetDecay(0);
        ps.push_back(lhef::Particle(24, 1, 2, 0, 0, 0, pW1->Px(), pW1->Py(),
                                    pW1->Pz(), pW1->E(), pW1->M(), 0, 0));
        auto pV1 = t1decay.GetDecay(1);
        ps.push_back(lhef::Particle(5, 1, 2, 0, 0, 0, pV1->Px(), pV1->Py(),
                                    pV1->Pz(), pV1->E(), pV1->M(), 0, 0));

        t2decay.Generate();
        auto pW2 = t2decay.GetDecay(0);
        ps.push_back(lhef::Particle(-24, 1, 3, 0, 0, 0, pW2->Px(), pW2->Py(),
                                    pW2->Pz(), pW2->E(), pW2->M(), 0, 0));
        auto pV2 = t2decay.GetDecay(1);
        ps.push_back(lhef::Particle(-5, 1, 3, 0, 0, 0, pV2->Px(), pV2->Py(),
                                    pV2->Pz(), pV2->E(), pV2->M(), 0, 0));

        lhef::EventEntry entry;
        int i = 1;
        for (const auto &p : ps) {
            entry.insert({i, p});
            ++i;
        }
        lhef::EventInfo evinfo(7, 0, 0, 0, 0, 0);
        lhef::Event ev(evinfo, entry);
        outfile << ev << '\n';
    }

    outfile << lhef::closingLine();
    outfile.close();
}
