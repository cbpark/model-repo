#include "Pythia8/Pythia.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string appname = "ttbar";
    if (argc != 3) {
        std::cerr << " Usage: " << appname << " output nevent\n"
                  << " output - output file\n"
                  << " nevent - number of events\n"
                  << " ex) " << appname << " test.lhe 100 > test.log\n";
        return 1;
    }
    std::string lhefile(argv[1]);

    // Generator.
    Pythia8::Pythia pythia;
    pythia.readFile("ttbar.cmnd");

    // Switch off generation of steps subsequent to the process level one.
    pythia.readString("PartonLevel:all = off");

    // Create an LHAup object that can access relevant information in pythia.
    Pythia8::LHAupFromPYTHIA8 myLHA(&pythia.process, &pythia.info);

    // Open a file on which LHEF events should be stored, and write header.
    myLHA.openLHEF(lhefile);

    // Initialization.
    pythia.init();

    // Store initialization info in the LHAup object.
    myLHA.setInit();

    // Write out this initialization info on the file.
    myLHA.initLHEF();

    int nevent = std::atoi(argv[2]);
    // Loop over events.
    for (int i = 0; i != nevent; ++i) {
        // Generate an event.
        pythia.next();

        // Store event info in the LHAup object.
        myLHA.setEvent();

        // Write out this event info on the file.
        // With optional argument (verbose =) false the file is smaller.
        myLHA.eventLHEF();
    }

    // Statistics: full printout.
    pythia.stat();

    // Update the cross section info based on MC integration during run.
    myLHA.updateSigma();

    // Write endtag. Overwrite initialization info with new cross sections.
    myLHA.closeLHEF(true);
}
