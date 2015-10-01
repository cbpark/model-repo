#include "qqbar_zh_jet.h"

#include <fstream>
#include <iostream>
#include <string>
#include "Pythia8/Pythia.h"
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

    // Initialization of Pythia
    Pythia8::Pythia pythia;
    pythia.readFile("qqbar_zh_jet.cmnd");
    pythia.init();

    const int nevent = std::atoi(argv[2]);
    for (int ieve = 0; ieve != nevent; ++ieve) {
        if (!pythia.next()) continue;
        outfile << "-- # of event: " << ieve + 1 << '\n';
        auto event = pythia.event;
        auto hadronLevel = getHadronLevelData(event);
        auto jetLevel = reconstructObjects(hadronLevel.first);
        double missingET = hadronLevel.second;
        outfile << missingET << '\n';
    }

    pythia.stat();
    outfile.close();
}
