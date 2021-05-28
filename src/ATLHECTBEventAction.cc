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

//Define constructor
//
ATLHECTBEventAction::ATLHECTBEventAction()
    : G4UserEventAction(),
    PDGID(0),
    vertexkenergy(0.),
    eleakage(0.),
    edep(0.),
    ecryostat(0.),
    elAr(0.),
    BirkelAr(0.),
    BirkeSlice{}
{}

//Define deconstructor
//
ATLHECTBEventAction::~ATLHECTBEventAction() {}

//Define BeginOfEventAction() and EndOfEventAction() methods
//
void ATLHECTBEventAction::BeginOfEventAction( const G4Event* ){
    
    //Initialize variables per event
    //
    PDGID =  0;
    vertexkenergy = 0.;
    eleakage = 0.;
    edep = 0.;
    ecryostat = 0.;
    elAr = 0.;
    BirkelAr = 0.;
    BirkeSlice.clear();
    for ( unsigned int i = 0; i<40; i++) { BirkeSlice.push_back(0.); }

}

void ATLHECTBEventAction::EndOfEventAction( const G4Event* ) {

    //Access Event random seeds
    //
    //auto rndseed = G4RunManager::GetRunManager()->GetRandomNumberStatusForThisEvent();
    
    //Accumulate statistics
    //
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleIColumn(0, PDGID);
    analysisManager->FillNtupleDColumn(1, vertexkenergy); 
    analysisManager->FillNtupleDColumn(2, eleakage);
    analysisManager->FillNtupleDColumn(3, edep);
    analysisManager->FillNtupleDColumn(4, ecryostat);
    analysisManager->FillNtupleDColumn(5, elAr);
    analysisManager->FillNtupleDColumn(6, BirkelAr);
    analysisManager->AddNtupleRow();
    
}

//**************************************************
