//**************************************************
// \file ATLHECTBanalysis1_v2p4.C
// \brief: Analysis #1 of ATLHECTB v2.4 
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 17 January 2022
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

void ATLHECTBanalysis1_v2p4(){
     
    // Analysis of e- data
    // energies 20, 40, 50, 80, 100, 119.1, 147.8 GeV
    //
    vector<double> emenergies = {20.,40.,50.,80.,100.,119.1,147.8};
    vector<string> emfiles;
    for ( unsigned int i=11; i<18; i++ ){
        emfiles.push_back( "ATLHECTBout_Run"+std::to_string(i)+".root" );
    }
    emanalysis( emenergies, emfiles );
    //Reconstrcuted energies for em runs (G4.10.7.p03, v2.4)
    //For missing energy points (30, 60, 120, 180 and 200 GeV) using 0.99*beamenergy
    //
    vector<double> recemenergies = {19.7811,0.99*30.,39.5788,49.5078,0.99*60.,79.2465,99.0562,117.983,146.417,0.99*180.,0.99*200};

    // Analysis of pi- data
    // energies 20, 30, 40, 50, 60, 80, 100, 120, 150, 180, 200 GeV
    //
    vector<double> pienergies = {20.,30.,40.,50.,60.,80.,100.,120.,150.,180.,200.};
    vector<string> pifiles;
    for ( unsigned int i=0; i<11; i++ ){
        pifiles.push_back( "ATLHECTBout_Run"+std::to_string(i)+".root" );
    }
    pianalysis( pienergies, pifiles, recemenergies );

    //Analysis to select channels for pi- analysis
    //
    //picalibrate(180., "ATLHECTBout_Run9.root");
    
    //Analysis to select channels for e- analysis
    //
    //ecalibrate(147.8,"ATLHECTBout_Run17.root");    

}

//**************************************************
