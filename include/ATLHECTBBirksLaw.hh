//**************************************************
// \file ATLHECTBBirksLaw.hh
// \brief: Definition of the ApplyBirks() function
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

#include <iostream>
#include "G4SystemOfUnits.hh"

G4double ApplyBirks( G4double de, G4double steplength ){

    const G4double Emin = 1.51;        //(MeV cm2)/g, minimal energy loss in LAr
    const G4double Ef = 9.2;           //kV/cm, electric field in LAr gaps  
    const G4double k = 0.0486;         //kV/cm g/(MeV cm2), paramter in Birks equation
    const G4double A = 1.+(k/Ef)*Emin; //Parameter in Birks equation
    const G4double LArdensity = 1.396; //g/cm3

    G4double satde;
    satde = de*A/( 1.+( (k/Ef)*(1/LArdensity)*(de/steplength) ) ); 
    
    G4cout<<"first de: "<<de<<" stepl: "<<steplength*0.1<<" last de: "<<satde<<G4endl;

    return satde;

}

//**************************************************
