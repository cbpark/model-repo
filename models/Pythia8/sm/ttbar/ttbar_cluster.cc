#include "Pythia8/Pythia.h"
#include <iostream>
#include <string>

void pythiaSettings(Pythia8::Pythia *pythia) {
    // Process selection.
    pythia->readString("Top:gg2ttbar = on");
    pythia->readString("Top:qqbar2ttbar = on");

    // parameters
    pythia->readString("5:m0 = 4.3");
    pythia->readString("6:m0 = 173.5");
    pythia->readString("6:onMode = off");
    pythia->readString("6:onIfAny = 24");
    pythia->readString("6:offIfAny = 1 3");
    pythia->readString("24:m0 = 80.385");
    pythia->readString("24:onMode = off");
    pythia->readString("24:onIfAny = 11 13");
}

int main(int argc, char* argv[]) {
    std::string appname = "ttbar";
    if (argc != 5) {
        std::cerr << " Usage: " << appname << " output nevent\n"
                  << " output - output file\n"
                  << " nevent - number of events\n"
                  << " jobid  - SGE array job(task) id\n"
                  << " jobidmax - dim of SGE array job\n"
                  << " ex) " << appname << " test.lhe 100 $SGE_TASK_ID $SGE_TASK_LAST > test.log\n";
		return 1;
    }
    std::string lhefile(argv[1]);

    int nevent = std::atoi(argv[2]);
    // Array job ID (1..Ncores) for cluster run
    int jobid = std::atoi(argv[3]);
    int jobidmax = std::atoi(argv[4]);

    // Generator.
    Pythia8::Pythia pythia;
    pythiaSettings(&pythia);

    // Switch off generation of steps subsequent to the process level one.
    // (These will not be stored anyway, so only steal time.)
    pythia.readString("PartonLevel:all = off");

    // Random number seed
    pythia.readString("Random:setSeed = on");
    pythia.readString("Random:seed = "+std::string(argv[3]));

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

/*------------------------------------------------------------------------
// Total jobidmax-jobs in a cluster will generate events 
// with assigned event IDs as follows :

// => nevent = total number of events to be generated
// => jobidmax = s_set_max; total number array jobs
// => jobid ; current ID of array job [1..j_max]
// => nevent_job = [nevent/(jobidmax-1)] ; number of event generation / a job
// => nevent_r = modulus of Nt/(jobidmax-1), [0..jobidmax-2]
// => nevent = nevet_job * (jobidmax-1) + nevent_r

// ######################################################
// (jobidmax-1)-jobs generating nevent_job events
//   if jobid =< jobidmax-1
//       jobid = 1..(jobidmax-1) : for evenets from (jobid-1)*nevent_job+1 to jobid*nevent_job
//       n1 = (jobid-1)*nevent_job+1
//       n2 = jobid*nevent_job

// #######################################################
// The last job generating nevent-r events
//   if jobid == jobidmax
//       jobid = jobidmax : for events from jobid*nevent_job+1 to nevent 
//       n1 = jobid*nevent_job+1
//       n2 = nevent
//-----------------------------------------------------------------------*/

    int nevent_job = nevent/(jobidmax-1);
//  int nevent_r = nevent%(jobidmax-1);

    int nevent_start = (jobid-1)*nevent_job+1;
    int nevent_end(0);

    if (jobid <= jobidmax - 1){
        nevent_end = jobid*nevent_job;
    }else if(jobid == jobidmax){
        nevent_end = nevent;
    }
    
    // Loop over events with jobid
    for (int i = nevent_start; i < nevent_end+1; ++i){
    //for (int i = 0; i < nevent; ++i) {

        cout << "event id = " << i << " from jobid = " << jobid << endl; 

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
