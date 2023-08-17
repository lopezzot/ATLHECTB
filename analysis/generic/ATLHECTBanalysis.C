//**************************************************
// \file ATLHECTBanalysis.C
// \brief: Generic analysis of ATLHECTB data 
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 17 August 2023
//**************************************************

//Usage: root 'ATLHECTBanalysis.C(folderpath/,version,g4version,physicslist)'
//e.g. root -l 'ATLHECTBanalysis.C("Data1/","2.6","11.1.ref05","FTFP_BERT")'
//
//If hadronic data are generated with the Fluka interface add "true"
//as the last macro argument

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

void ATLHECTBanalysis(const string folder,const string version,const string g4version,const string pl, const bool isFluka=false){
 
    //This is a generic analysis therefore the legend for MC data
    //must be taken as input
    //e.g. "#splitline{ATLHECTB v2.6 }{Geant4.11.1.ref05 FTFP_BERT }"
    //
    const string emMClegend = "#splitline{ATLHECTB "+version+ "}{Geant4-"+g4version+" "+pl+ "}";
    string piMClegend = emMClegend;
    if(isFluka) piMClegend = "#splitline{ATLHECTB "+version+ "}{FLUKA}";

    // Analysis of e- data
    // energies 20, 40, 50, 80, 100, 119.1, 147.8 GeV
    //
    vector<double> emenergies = {20.,40.,50.,80.,100.,119.1,147.8};
    vector<string> emfiles;
    for ( unsigned int i=11; i<18; i++ ){
        emfiles.push_back( "ATLHECTBout_Run"+std::to_string(i)+".root" );
    }
    //First call to emanalysis to extract emdata, using dummy 44.95 value for SF
    //will be called again later with correct value
    emoutput emdata = emanalysis( emenergies, emfiles, folder, emMClegend );
    //Second call to emanalysis with correct SF value, final plots are
    //now recreated and correct
    emdata = emanalysis( emenergies, emfiles, folder, emMClegend, emdata.avgSF*10. );
    emdata.print();

    //Reconstrcuted energies for em runs
    //For missing energy points (30, 60, 120, 180 and 200 GeV) using 0.99*beamenergy
    //
    vector<double> recemenergies = {emdata.recenergies.at(0),0.99*30.,emdata.recenergies.at(1),
        emdata.recenergies.at(2),0.99*60.,emdata.recenergies.at(3),emdata.recenergies.at(4),
        emdata.recenergies.at(5),emdata.recenergies.at(6),0.99*180.,0.99*200}; 

    // Analysis of pi- data
    // energies 20, 30, 40, 50, 60, 80, 100, 120, 150, 180, 200 GeV
    //
    vector<double> pienergies = {20.,30.,40.,50.,60.,80.,100.,120.,150.,180.,200.};
    vector<string> pifiles;
    for ( unsigned int i=0; i<11; i++ ){
        pifiles.push_back( "ATLHECTBout_Run"+std::to_string(i)+".root" );
    }
    //Last argument is FLUKA(bool). If true assumes the g4-to-fluka interface
    //was used to generate the data.
    //
    pianalysis( pienergies, pifiles, recemenergies, folder, emdata.avgSF*10., piMClegend, isFluka);

    //Analysis to select channels for pi- analysis
    //
    //picalibrate(180., "ATLHECTBout_Run9.root");

    //Analysis to select channels for e- analysis
    //
    //ecalibrate(147.8,"ATLHECTBout_Run17.root"); 

}

//**************************************************
