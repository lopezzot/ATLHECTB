//**************************************************
// \file ATLHECTBActionInitialization.hh
// \brief: Definition of ATLTBHECActionInitialization class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

// Prevent including header multiple times
//
#ifndef ATLHECTBActionInitialization_h
#  define ATLHECTBActionInitialization_h 1

// Includers from Geant4
//
#  include "G4VUserActionInitialization.hh"

class ATLHECTBDetectorConstruction;

class ATLHECTBActionInitialization : public G4VUserActionInitialization
{
  public:
    ATLHECTBActionInitialization(/*ATLHECTBDetectorConstruction**/);
    virtual ~ATLHECTBActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

    // private:
    // ATLHECTBDetectorConstruction* fDetConstruction;
};

#endif

//**************************************************
