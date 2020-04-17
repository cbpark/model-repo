/*
 *  Copyright 2012 Chan Beom Park <cbpark@gmail.com>
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include "Pythia8/Pythia.h"

int main(int argc, char *argv[]) {
    std::string appname = "pph2hpw_2l";
    if (argc != 4) {
        std::cerr << " Usage: " << appname << " card output nevent\n"
                  << " card   - the Pythia8 card\n"
                  << " output - the output file\n"
                  << " nevent - the number of events\n"
                  << " ex) " << appname
                  << " pythia8.cmnd event.lhe 100 > event.log\n";
        return 1;
    }

    Pythia8::Pythia pythia;
    std::string cardfile(argv[1]);
    pythia.readFile(cardfile);

    pythia.readString("HiggsBSM:gg2H2 = on");
    pythia.readString("HiggsBSM:ffbar2H2 = on");

    pythia.readString("35:onMode = off");
    pythia.readString("35:onIfMatch = 24 -37");  // H --> H+ W-
    pythia.readString("35:onIfMatch = -24 37");

    pythia.readString("37:onMode = off");
    pythia.readString("37:onIfAny = 5 6");  // H+ --> t b

    pythia.readString("6:m0 = 173.0");
    pythia.readString("6:onMode = off");
    pythia.readString("6:onIfMatch = 24 5");  // t --> b W

    pythia.readString("24:m0 = 80.379");
    pythia.readString("24:onMode = off");
    pythia.readString("24:onIfAny = 11 13");  // leptonic decays of the W boson

    // Switch off generation of steps subsequent to the process level one.
    pythia.readString("PartonLevel:all = off");

    // Create an LHAup object that can access relevant information in pythia.
    Pythia8::LHAupFromPYTHIA8 myLHA(&pythia.process, &pythia.info);

    // Open a file on which LHEF events should be stored, and write header.
    std::string lhefile(argv[2]);
    myLHA.openLHEF(lhefile);

    // Initialization.
    pythia.init();

    // Store initialization info in the LHAup object.
    myLHA.setInit();

    // Write out this initialization info on the file.
    myLHA.initLHEF();

    int nevent = std::atoi(argv[3]);
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
