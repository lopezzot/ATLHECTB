//**************************************************
// \file ATLHECTBanalysis1_v2p6.C
// \brief: Analysis #1 of ATLHECTB v2.6 
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 5 July 2023
//**************************************************

//Includers from C++
//
#include <string>
#include <array>

//Includers from project files
//
#include "emanalysis.h"
#include "pianalysis.h"
#include "ecalibrate.h"
#include "picalibrate.h"
#include "FLUKA_comparison.h"

void ATLHECTBanalysis1_v2p6(bool FLUKA_Comparison = false){
     
    // Analysis of e- data
    // energies 20, 40, 50, 80, 100, 119.1, 147.8 GeV
    //
    vector<double> emenergies = {20.,40.,50.,80.,100.,119.1,147.8};
    vector<string> emfiles;
    for ( unsigned int i=11; i<18; i++ ){
        emfiles.push_back( "ATLHECTBout_Run"+std::to_string(i)+".root" );
    }
    emanalysis( emenergies, emfiles );

    //Reconstrcuted energies for em runs (G4.11.1.ref05, v2.6)
    //For missing energy points (30, 60, 120, 180 and 200 GeV) using 0.99*beamenergy
    //
    vector<double> recemenergies = {19.833,0.99*30.,39.6916,49.6423,0.99*60.,79.418,99.2707,118.209,146.685,0.99*180.,0.99*200}; //**Updated (G4.11.1.ref05 v2.6) **

    // Analysis of pi- data
    // energies 20, 30, 40, 50, 60, 80, 100, 120, 150, 180, 200 GeV
    //
    vector<double> pienergies = {20.,30.,40.,50.,60.,80.,100.,120.,150.,180.,200.};
    vector<string> pifiles;
    for ( unsigned int i=0; i<11; i++ ){
        pifiles.push_back( "ATLHECTBout_Run"+std::to_string(i)+".root" );
    }
    if (FLUKA_Comparison) {
        pianalysis(pienergies, pifiles, recemenergies, false);
        pianalysis(pienergies, pifiles, recemenergies, true);
        FLUKA_comparison();
    }
    else {
        pianalysis( pienergies, pifiles, recemenergies, false);
    }

    //Analysis to select channels for pi- analysis
    //
    //picalibrate(180., "ATLHECTBout_Run9.root");

    //Analysis to select channels for e- analysis
    //
    //ecalibrate(147.8,"ATLHECTBout_Run17.root"); 

}

//**************************************************
