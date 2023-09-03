//**************************************************
// \file ATLHECTBRunAction.cc
// \brief: Implementation of ATLHECTBRunAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

// Includers from project files
//
#include "ATLHECTBRunAction.hh"

#include "ATLHECTBEventAction.hh"
#ifdef ATLHECTB_LEAKANALYSIS
#  include "SpectrumAnalyzer.hh"
#endif

// Includers from Geant4
//
// #include "g4root.hh" //not avaibale from Geant4 11.0 on
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Version.hh"
#if G4VERSION_NUMBER < 1100
#  include "g4root.hh"
#else
#  include "G4AnalysisManager.hh"
#endif
#include <string>

// Define constructor
//
ATLHECTBRunAction::ATLHECTBRunAction(ATLHECTBEventAction* eventAction)
  : G4UserRunAction(), fEventAction(eventAction)
{
  G4RunManager::GetRunManager()->SetPrintProgress(1);  // print each event number

  // Instantiate analysis manager
  auto analysisManager = G4AnalysisManager::Instance();  // using ROOT
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(1);

  analysisManager->CreateNtuple("ATLHECTBout", "ATLHECTBoutput");
  analysisManager->CreateNtupleIColumn("PDGID");
  analysisManager->CreateNtupleDColumn("vertexkenergy");
  analysisManager->CreateNtupleDColumn("eleakage");
  analysisManager->CreateNtupleDColumn("edep");
  analysisManager->CreateNtupleDColumn("ecryostat");
  analysisManager->CreateNtupleDColumn("elAr");
  analysisManager->CreateNtupleDColumn("BirkelAr");
  analysisManager->CreateNtupleDColumn("M1L1BirkeLayer", fEventAction->GetM1L1BirkeLayer());
  analysisManager->CreateNtupleDColumn("M2L1BirkeLayer", fEventAction->GetM2L1BirkeLayer());
  analysisManager->CreateNtupleDColumn("M3L1BirkeLayer", fEventAction->GetM3L1BirkeLayer());
  analysisManager->CreateNtupleDColumn("M1L2BirkeLayer", fEventAction->GetM1L2BirkeLayer());
  analysisManager->CreateNtupleDColumn("M2L2BirkeLayer", fEventAction->GetM2L2BirkeLayer());
  analysisManager->CreateNtupleDColumn("M3L2BirkeLayer", fEventAction->GetM3L2BirkeLayer());
  analysisManager->CreateNtupleDColumn("M1L3BirkeLayer", fEventAction->GetM1L3BirkeLayer());
  analysisManager->CreateNtupleDColumn("M2L3BirkeLayer", fEventAction->GetM2L3BirkeLayer());
  analysisManager->CreateNtupleDColumn("M3L3BirkeLayer", fEventAction->GetM3L3BirkeLayer());
  analysisManager->CreateNtupleDColumn("M1L4BirkeLayer", fEventAction->GetM1L4BirkeLayer());
  analysisManager->CreateNtupleDColumn("M2L4BirkeLayer", fEventAction->GetM2L4BirkeLayer());
  analysisManager->CreateNtupleDColumn("M3L4BirkeLayer", fEventAction->GetM3L4BirkeLayer());
  analysisManager->FinishNtuple();

#ifdef ATLHECTB_LEAKANALYSIS
  SpectrumAnalyzer::GetInstance()->CreateNtupleAndScorer("ke");
#endif
}

// Define deconstructor
//
ATLHECTBRunAction::~ATLHECTBRunAction()
{
#if G4VERSION_NUMBER < 1100
  delete G4AnalysisManager::Instance();  // not needed for G4 v11 and up
#endif
}

// Define BeginOfRunAction() and EndOfRunAction() methods
//
void ATLHECTBRunAction::BeginOfRunAction(const G4Run* Run)
{
  // Inform RunManager to save random seeds
  //
  // G4RunManager::GetRunManager()->SetRandomNumberStore( true );

  auto analysisManager = G4AnalysisManager::Instance();

  std::string runnumber = std::to_string(Run->GetRunID());
  G4String outputfile = "ATLHECTBout_Run" + runnumber + ".root";
  analysisManager->OpenFile(outputfile);
}

void ATLHECTBRunAction::EndOfRunAction(const G4Run*)
{
  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->Write();
  analysisManager->CloseFile();
}

//**************************************************
