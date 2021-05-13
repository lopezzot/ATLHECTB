//**************************************************
// \file ATLHECTBRunAction.cc 
// \brief: Implementation of ATLHECTBRunAction class 
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

//Includers from project files
//
#include "ATLHECTBRunAction.hh"

//Includers from Geant4
//
#include "g4root.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//Define constructor
//
ATLHECTBRunAction::ATLHECTBRunAction()
    : G4UserRunAction() {
    
    G4RunManager::GetRunManager()->SetPrintProgress( 1 ); //print each event number

    //Instantiate analysis manager
    auto analysisManager = G4AnalysisManager::Instance(); //using ROOT
    analysisManager->SetVerboseLevel( 1 );
    analysisManager->SetNtupleMerging( 1 );    
}

//Define deconstructor
//
ATLHECTBRunAction::~ATLHECTBRunAction(){
    
    delete G4AnalysisManager::Instance();

}

//Define BeginOfRunAction() and EndOfRunAction() methods
//
void ATLHECTBRunAction::BeginOfRunAction( const G4Run* ){

    //G4RunManger::GetRunManager()->SetRandomNumberStore( true ); //interesting, to be used
    auto analysisManager = G4AnalysisManager::Instance();

    G4String outputfile = "ATLHECTB";
    analysisManager->OpenFile( outputfile );
}

void ATLHECTBRunAction::EndOfRunAction( const G4Run* ){
    
    auto analysisManager = G4AnalysisManager::Instance();
    
    analysisManager->Write();
    analysisManager->CloseFile();

}

//**************************************************
