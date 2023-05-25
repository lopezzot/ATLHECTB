//**************************************************
// \file ATLHECTBActionInitialization.cc
// \brief: Implementation of ATLTBHECActionInitialization class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

// Includers from project files
//
#include "ATLHECTBActionInitialization.hh"

#include "ATLHECTBDetectorConstruction.hh"
#include "ATLHECTBEventAction.hh"
#include "ATLHECTBPrimaryGeneratorAction.hh"
#include "ATLHECTBRunAction.hh"
#include "ATLHECTBSteppingAction.hh"

// Define constructor
//
ATLHECTBActionInitialization::ATLHECTBActionInitialization(
  /*ATLHECTBDetectorConstruction* detConstruction*/)
  : G4VUserActionInitialization()  //,
/*fDetConstruction( detConstruction )*/
{}

// Define deconstruction
//
ATLHECTBActionInitialization::~ATLHECTBActionInitialization() {}

// Define Build() and BuildForMaster() methods
//
void ATLHECTBActionInitialization::BuildForMaster() const
{
  auto eventAction = new ATLHECTBEventAction;
  SetUserAction(new ATLHECTBRunAction(eventAction));
}

void ATLHECTBActionInitialization::Build() const
{
  SetUserAction(new ATLHECTBPrimaryGeneratorAction);
  auto eventAction = new ATLHECTBEventAction;
  SetUserAction(new ATLHECTBRunAction(eventAction));
  SetUserAction(eventAction);
  SetUserAction(new ATLHECTBSteppingAction(/*fDetConstruction,*/ eventAction));
}

//**************************************************
