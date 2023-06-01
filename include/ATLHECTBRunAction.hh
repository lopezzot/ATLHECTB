//**************************************************
// \file ATLHECTBRunAction.hh
// \brief: Definition of ATLHECTBRunAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

// Prevent including header multiple times
//
#ifndef ATLHECTBRunAction_h
#  define ATLHECTBRunAction_h 1

// Includers from Geant4
//
#  include "G4UserRunAction.hh"
#  include "globals.hh"

class ATLHECTBEventAction;

class G4Run;

class ATLHECTBRunAction : public G4UserRunAction
{
  public:
    ATLHECTBRunAction(ATLHECTBEventAction* eventAction);
    virtual ~ATLHECTBRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

  private:
    ATLHECTBEventAction* fEventAction;
};

#endif

//**************************************************
