//**************************************************
// \file ATLHECTBanalysis1_v1p4.cc
// \brief: Analysis #1 of ATLHECTB v1.4 
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 21 September 2021
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

void ATLHECTBanalysis1_v1p4(){
     
    // Analysis of e- data
    // energies 20, 40, 50, 80, 100, 119.1, 147.8 GeV
    //
    vector<double> emenergies = {20.,40.,50.,80.,100.,119.1,147.8};
    vector<string> emfiles;
    for ( unsigned int i=11; i<18; i++ ){
        emfiles.push_back( "ATLHECTBout_Run"+std::to_string(i)+".root" );
    }
    emanalysis( emenergies, emfiles );
     
    // Analysis of pi- data
    // energies 20, 30, 40, 50, 60, 80, 100, 120, 150, 180, 200 GeV
    //
    vector<double> pienergies = {20.,30.,40.,50.,60.,80.,100.,120.,150.,180.,200.};
    vector<string> pifiles;
    for ( unsigned int i=0; i<11; i++ ){
        pifiles.push_back( "ATLHECTBout_Run"+std::to_string(i)+".root" );
    }
    pianalysis( pienergies, pifiles );

    //Analysis to select channels for pi- analysis
    //
    //picalibrate(180., "ATLHECTBout_Run9.root");
    
    //Analysis to select channels for e- analysis
    //
    //ecalibrate(147.8,"ATLHECTBout_Run17.root");    

}

//**************************************************
