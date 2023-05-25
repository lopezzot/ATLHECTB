//**************************************************
// \file ATLHECTBSteppingAction.hh
// \brief: Definition of ATLHECTBSteppingAction.hh
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

// Prevent including header multiple times
//
#ifndef ATLHECTBSteppingAction_h
#  define ATLHECTBSteppingAction_h 1

// Includers from Geant4
//
#  include "G4UserSteppingAction.hh"

// Includers from project files
//
#  include "ATLHECTBSignalCalculator.hh"

class ATLHECTBDetectorConstruction;
class ATLHECTBEventAction;

class ATLHECTBSteppingAction : public G4UserSteppingAction
{
  public:
    ATLHECTBSteppingAction(  // const ATLHECTBDetectorConstruction* detConstruction,
      ATLHECTBEventAction* eventAction);
    virtual ~ATLHECTBSteppingAction();

    virtual void UserSteppingAction(const G4Step* step);

  private:
    // const ATLHECTBDetectorConstruction* fDetConstruction;
    ATLHECTBEventAction* fEventAction;

    // pointer to ATLHECTSignalCalculator (singleton)
    ATLHECTBSignalCalculator* fSCalculator;
};

#endif

//**************************************************
