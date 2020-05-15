#include "pph2tt_2l_jet.h"

#include "hadron_level_data.h"
#include "jet_level_data.h"

#include "Pythia8/Pythia.h"
#include "lhco/lhco.h"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string appname = "pph2tt_jet";
    if (argc != 3) {
        std::cerr << " Usage: " << appname << " output nevent\n"
                  << " output - output file\n"
                  << " nevent - number of events\n"
                  << " ex) " << appname << " test.lhco 100 > test.log\n";
        return 1;
    }
    std::ofstream outfile(argv[1]);
    if (!outfile.is_open()) {
        std::cerr << "File open error: " << argv[1] << '\n';
        return 1;
    }
    outfile << lhco::openingLine() << '\n';

    // Initialization of Pythia
    Pythia8::Pythia pythia;
    pythia.readFile("pph2tt.cmnd");

    pythia.readString("HiggsBSM:gg2H2 = on");
    pythia.readString("HiggsBSM:ffbar2H2 = on");

    pythia.readString("35:onMode = off");
    pythia.readString("35:onIfMatch = 6 -6");  // H --> t tbar

    pythia.readString("6:m0 = 173.0");
    pythia.readString("6:onMode = off");
    pythia.readString("6:onIfMatch = 24 5");  // t --> b W

    pythia.readString("24:m0 = 80.379");
    pythia.readString("24:onMode = off");
    pythia.readString("24:onIfAny = 11 13");  // leptonic decays of the W boson

    std::string pdfset("LHAPDF6:NNPDF31_lo_as_0118");
    pythia.readString("PDF:pSet = " + pdfset);

    pythia.init();

    const int nevent = std::atoi(argv[2]);
    for (int ieve = 0; ieve != nevent; ++ieve) {
        if (!pythia.next()) continue;

        auto event = pythia.event;
        auto hadronLevel = getHadronLevelData(event);
        auto jetLevel = reconstructObjects(hadronLevel, 0.4);

        lhco::Header header(ieve + 1, 0);
        lhco::Objects objs = jetLevelToLHCO(jetLevel);
        lhco::RawEvent ev(header, objs);
        outfile << ev << '\n';
    }

    pythia.stat();
    outfile.close();
}
