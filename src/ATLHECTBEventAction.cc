//**************************************************
// \file ATLHECTBEventAction.cc
// \brief: Implementation of ATLHECTBEventAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

//Includera from project files
//
#include "ATLHECTBEventAction.hh"
#include "ATLHECTBRunAction.hh"

//Includers from Geant4
//
#include "g4root.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"
#include <iomanip>

//Define constructor
//
ATLHECTBEventAction::ATLHECTBEventAction()
    : G4UserEventAction() {}

//Define deconstructor
//
ATLHECTBEventAction::~ATLHECTBEventAction() {}

//Define BeginOfEventAction() and EndOfEventAction() methods
//
void ATLHECTBEventAction::BeginOfEventAction( const G4Event* ){}

void ATLHECTBEventAction::EndOfEventAction( const G4Event* event ) {

    //G4cout<< "--->End of Event: " << event->GetEventID() <<G4endl;

}

//**************************************************
