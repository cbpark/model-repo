#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"
#include <iostream>
#include <string>

void PythiaSettings(Pythia8::Pythia *pythia) {
    // No automatic event listings - do it manually below.
    pythia->readString("Next:numberShowLHA = 0");
    pythia->readString("Next:numberShowInfo = 0");
    pythia->readString("Next:numberShowProcess = 0");
    pythia->readString("Next:numberShowEvent = 0");

    // Initialize Les Houches Event File run.
    pythia->readString("Beams:frameType = 4");
}

int main(int argc, char* argv[]) {
    std::string appname = "LHE2HepMC";
    if (argc != 3) {
        std::cerr << " Usage: " << appname << " input" << " nevent\n"
                  << " input - input file in LHE format\n"
                  << "         the output file will input.hepmc\n"
                  << " nevent - number of events to produce\n"
                  << "          You can find the nevent by grep -cw '<event>'\n"
                  << " ex) " << appname << " input.lhe 100 >"
                  << " pythia_hepmc.log\n";
        return 1;
    }

    std::string lhefile(argv[1]);
    unsigned fname = lhefile.find_last_of(".\\");
    if (lhefile.substr(fname + 1).compare("lhe") != 0) {
        std::cout << " It seems that the input is not the LHEF file.\n"
                  << " Check the input file.\n";
        return 1;
    }

    // Interface for conversion from Pythia8::Event to HepMC event.
    HepMC::Pythia8ToHepMC ToHepMC;

    // Specify file where HepMC events will be stored.
    std::string hepmcfile(argv[1]);
    hepmcfile = hepmcfile.substr(0, hepmcfile.size() - 4);
    hepmcfile.append(".hepmc");
    HepMC::IO_GenEvent ascii_io(hepmcfile, std::ios::out);

    // Generator.
    Pythia8::Pythia pythia;
    pythia.readString("Beams:LHEF = " + lhefile);
    PythiaSettings(&pythia);
    pythia.init();

    int nEvent = std::atoi(argv[2]);
    int nAbort = 1000;
    int iAbort = 0;
    // Begin event loop. Generate event. Skip if error.
    for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
        // Generate event.
        if (!pythia.next()) {
            // If failure because reached end of file then exit event loop.
            if (pythia.info.atEndOfFile()) {
                std::cout << " Aborted since"
                          << " reached end of Les Houches Event File\n";
                break;
            }

            // First few failures write off as "acceptable" errors, then quit.
            if (++iAbort < nAbort) continue;
            std::cout << " Event generation aborted prematurely,"
                      << " owing to error!\n";
            break;
        }

        // Construct new empty HepMC event and fill it.
        // Units will be as chosen for HepMC build; but can be changed
        // by arguments, e.g. GenEvt( HepMC::Units::GEV, HepMC::Units::MM)
        HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
        ToHepMC.fill_next_event(pythia, hepmcevt);

        // Write the HepMC event to file. Done with it.
        ascii_io << hepmcevt;
        delete hepmcevt;

        // End of event loop. Statistics. Histogram.
    }
    pythia.stat();
}
