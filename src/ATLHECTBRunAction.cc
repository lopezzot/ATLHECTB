//**************************************************
// \file ATLHECTBRunAction.cc 
// \brief: Implementation of ATLHECTBRunAction class 
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

//Includers from project files
//
#include "ATLHECTBRunAction.hh"
#include "ATLHECTBEventAction.hh"

//Includers from Geant4
//
#include "g4root.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <string>

//Define constructor
//
ATLHECTBRunAction::ATLHECTBRunAction( ATLHECTBEventAction* eventAction )
    : G4UserRunAction(),
      fEventAction( eventAction ) {
    
    G4RunManager::GetRunManager()->SetPrintProgress( 1 ); //print each event number

    //Instantiate analysis manager
    auto analysisManager = G4AnalysisManager::Instance(); //using ROOT
    analysisManager->SetVerboseLevel( 1 );
    analysisManager->SetNtupleMerging( 1 );    

    analysisManager->CreateNtuple("ATLHECTBout", "ATLHECTBoutput");
    analysisManager->CreateNtupleIColumn("PDGID");
    analysisManager->CreateNtupleDColumn("vertexkenergy");
    analysisManager->CreateNtupleDColumn("eleakage");
    analysisManager->CreateNtupleDColumn("edep");
    analysisManager->CreateNtupleDColumn("ecryostat");
    analysisManager->CreateNtupleDColumn("elAr");
    analysisManager->CreateNtupleDColumn("BirkelAr");
    analysisManager->CreateNtupleDColumn("BirkeSlice", fEventAction->GetBirkeSlice() );
    analysisManager->FinishNtuple();

}

//Define deconstructor
//
ATLHECTBRunAction::~ATLHECTBRunAction(){
    
    delete G4AnalysisManager::Instance();

}

//Define BeginOfRunAction() and EndOfRunAction() methods
//
void ATLHECTBRunAction::BeginOfRunAction( const G4Run* Run){

    //Inform RunManager to save random seeds 
    //
    //G4RunManager::GetRunManager()->SetRandomNumberStore( true );

    auto analysisManager = G4AnalysisManager::Instance();

    std::string runnumber = std::to_string( Run->GetRunID() );
    G4String outputfile = "ATLHECTBout_Run"+runnumber;
    analysisManager->OpenFile( outputfile );
}

void ATLHECTBRunAction::EndOfRunAction( const G4Run* ){
    
    auto analysisManager = G4AnalysisManager::Instance();
    
    analysisManager->Write();
    analysisManager->CloseFile();

}

//**************************************************
