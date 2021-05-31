//**************************************************
// \file ATLHECTBBirksLaw.hh
// \brief: Definition of the ApplyBirks() function
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

G4double ApplyBirks( const G4double& de, const G4double& steplength ){

    const G4double Emin = 1.51;        //(MeV cm2)/g, minimal energy loss in LAr
    const G4double Ef = 9.2;           //kV/cm, electric field in LAr gaps  
    const G4double k = 0.0486;         //kV/cm g/(MeV cm2), paramter in Birks equation
    const G4double A = 1.+(k/Ef)*Emin; //Parameter in Birks equation
    const G4double LArdensity = 1.396; //g/cm3

    G4double satde = de*A/( 1.+( (k/Ef)*(1/LArdensity)*(de/steplength) ) ); 
    
    return satde;

}

//**************************************************
