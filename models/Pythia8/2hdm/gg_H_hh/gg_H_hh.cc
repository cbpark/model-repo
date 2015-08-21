#include "Pythia8/Pythia.h"
#include <iostream>
#include <string>

void pythiaSettings(Pythia8::Pythia *pythia) {
    // Process selection.
    pythia->readString("Higgs:useBSM = on");
    pythia->readString("HiggsBSM:gg2H2 = on");

    // parameters
    pythia->readString("35:m0 = 400.0");
    pythia->readString("35:onMode = off");
    pythia->readString("35:onIfAll 25");
    pythia->readString("HiggsH2:coup2W = 0.0");
    pythia->readString("HiggsH2:coup2Z = 0.0");
    pythia->readString("HiggsH2:coup2u = 0.000001");
    pythia->readString("HiggsH2:coup2d = 0.000001");
    pythia->readString("HiggsH2:coup2l = 0.0");
    pythia->readString("HiggsH2:coup2H1H1 = 0.07");
    pythia->readString("36:m0 = 1000.0");  // disable H_20 -> A Z

    pythia->readString("Higgs:cubicWidth = on");
    pythia->readString("25:m0 = 125.0");
    pythia->readString("25:onMode = off");
    pythia->readString("25:onIfAny = 5 24");

    pythia->readString("24:m0 = 80.385");
    pythia->readString("24:onMode = off");
    pythia->readString("24:onIfAny = 11 13");

    pythia->readString("5:m0 = 4.3");
    pythia->readString("6:m0 = 173.5");
    pythia->readString("15:m0 = 1.77682");
    pythia->readString("23:m0 = 91.1876");
}

int main(int argc, char* argv[]) {
    std::string appname = "gg_H_hh";
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
    pythiaSettings(&pythia);

    // Switch off generation of steps subsequent to the process level one.
    // (These will not be stored anyway, so only steal time.)
    pythia.readString("PartonLevel:all = off");

    // Create an LHAup object that can access relevant information in pythia.
    Pythia8::LHAupFromPYTHIA8 myLHA(&pythia.process, &pythia.info);

    // Open a file on which LHEF events should be stored, and write header.
    myLHA.openLHEF(lhefile);

    // LHC 14 TeV initialization.
    pythia.readString("Beams:eCM = 14000.0");
    pythia.init();

    // Store initialization info in the LHAup object.
    myLHA.setInit();

    // Write out this initialization info on the file.
    myLHA.initLHEF();

    int nevent = std::atoi(argv[2]);
    // Loop over events.
    for (int i = 0; i < nevent; ++i) {
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
