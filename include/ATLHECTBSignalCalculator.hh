//**************************************************
// \file ATLHECTBSignalCalculator.hh
// \brief: Definition of ATLHECTBSignalCalculator class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

// Prevent including header multiple times
//
#ifndef ATLHECTBSignalCalculator_h
#  define ATLHECTBSignalCalculator_h 1

// Includers from Geant4
//
#  include "globals.hh"

class ATLHECTBSignalCalculator
{
  private:
    static G4ThreadLocal ATLHECTBSignalCalculator* instance;

    // Private constructor (singleton)
    ATLHECTBSignalCalculator();

  public:
    static ATLHECTBSignalCalculator* Instance();

    G4double ApplyBirks(const G4double& de, const G4double& steplength);

    G4int IndexL1(const G4double& eta, const G4double& relX);
    G4int IndexL2(const G4double& eta, const G4double& relX);
    G4int IndexL3(const G4double& eta, const G4double& relX);
    G4int IndexL4(const G4double& eta, const G4double& relX);
};

#endif

//**************************************************
