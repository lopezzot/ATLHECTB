//**************************************************
// \file ATLHECTBSignalCalculator.hh
// \brief: Definition of ATLHECTBSignalCalculator class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

#ifndef ATLHECTBSignalCalculator_h
#define ATLHECTBSignalCalculator_h 1

#include "globals.hh"

class ATLHECTBSignalCalculator {

    private:

        static ATLHECTBSignalCalculator* instance;
 
        // Private constructor (singleton)
        ATLHECTBSignalCalculator(); 

    public:
        
        static ATLHECTBSignalCalculator* Instance();

        G4double ApplyBirks( const G4double& de, const G4double& steplength );

};

#endif

//**************************************************
