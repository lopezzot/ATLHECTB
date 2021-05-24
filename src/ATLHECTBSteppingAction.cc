//**************************************************
// \file ATLHECTBSteppingAction.cc
// \brief: Implementation of ATLHECTBSteppingAction.cc
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

//Includers from project files
//
#include "ATLHECTBSteppingAction.hh"
#include "ATLHECTBEventAction.hh"
#include "ATLHECTBDetectorConstruction.hh"

//Includers from Geant4
//
#include "G4Step.hh"
#include "G4RunManager.hh"

//Definition of constructor
//
ATLHECTBSteppingAction::ATLHECTBSteppingAction(
        const ATLHECTBDetectorConstruction* detConstruction,
        ATLHECTBEventAction* eventAction )
    : G4UserSteppingAction(),
      fDetConstruction( detConstruction ),
      fEventAction( eventAction ) {}

//Definition of deconstructor
//
ATLHECTBSteppingAction::~ATLHECTBSteppingAction(){}

//Definition of UserSteppingAction() method
//
void ATLHECTBSteppingAction::UserSteppingAction(const G4Step* step){
    
}

//**************************************************
