#include "qqbar_zh_jet.h"
#include <fstream>
#include <iostream>
#include <string>
#include "Pythia8/Pythia.h"
#include "CLHCO/lhco.h"
#include "hadron_level_data.h"
#include "jet_level_data.h"

int main(int argc, char* argv[]) {
    std::string appname = "qqbar_zh_jet";
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
    pythia.readFile("qqbar_zh_jet.cmnd");
    pythia.init();

    const int nevent = std::atoi(argv[2]);
    for (int ieve = 0; ieve != nevent; ++ieve) {
        if (!pythia.next()) continue;

        auto event = pythia.event;
        auto hadronLevel = getHadronLevelData(event);
        auto jetLevel = reconstructObjects(hadronLevel);

        lhco::Header header(ieve + 1, 0);
        lhco::Objects objs = jetLevelToLHCO(jetLevel);
        lhco::RawEvent ev(header, objs);
        outfile << ev << '\n';
    }

    pythia.stat();
    outfile.close();
}
