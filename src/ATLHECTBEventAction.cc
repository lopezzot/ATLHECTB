//**************************************************
// \file ATLHECTBEventAction.cc
// \brief: Implementation of ATLHECTBEventAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

// Includers from project files
//
#include "ATLHECTBEventAction.hh"

#include "ATLHECTBRunAction.hh"
#ifdef ATLHECTB_LEAKANALYSIS
#  include "SpectrumAnalyzer.hh"
#endif

// Includers from Geant4
//
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4Version.hh"
#if G4VERSION_NUMBER < 1100
#  include "g4root.hh"
#else
#  include "G4AnalysisManager.hh"
#endif

// Define constructor
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
    M1L1BirkeLayer{},
    M2L1BirkeLayer{},
    M3L1BirkeLayer{},
    M1L2BirkeLayer{},
    M2L2BirkeLayer{},
    M3L2BirkeLayer{},
    M1L3BirkeLayer{},
    M2L3BirkeLayer{},
    M3L3BirkeLayer{},
    M1L4BirkeLayer{},
    M2L4BirkeLayer{},
    M3L4BirkeLayer{}
{}

// Define deconstructor
//
ATLHECTBEventAction::~ATLHECTBEventAction() {}

// Define BeginOfEventAction() and EndOfEventAction() methods
//
void ATLHECTBEventAction::BeginOfEventAction(const G4Event*)
{
  // Initialize variables per event
  //
  PDGID = 0;
  vertexkenergy = 0.;
  eleakage = 0.;
  edep = 0.;
  ecryostat = 0.;
  elAr = 0.;
  BirkelAr = 0.;
  M1L1BirkeLayer.clear();
  M2L1BirkeLayer.clear();
  M3L1BirkeLayer.clear();
  M1L2BirkeLayer.clear();
  M2L2BirkeLayer.clear();
  M3L2BirkeLayer.clear();
  M1L3BirkeLayer.clear();
  M2L3BirkeLayer.clear();
  M3L3BirkeLayer.clear();
  M1L4BirkeLayer.clear();
  M2L4BirkeLayer.clear();
  M3L4BirkeLayer.clear();

  for (unsigned int i = 0; i < 24; i++) {
    M1L1BirkeLayer.push_back(0.);
  }
  for (unsigned int i = 0; i < 24; i++) {
    M2L1BirkeLayer.push_back(0.);
  }
  for (unsigned int i = 0; i < 24; i++) {
    M3L1BirkeLayer.push_back(0.);
  }
  for (unsigned int i = 0; i < 23; i++) {
    M1L2BirkeLayer.push_back(0.);
  }
  for (unsigned int i = 0; i < 23; i++) {
    M2L2BirkeLayer.push_back(0.);
  }
  for (unsigned int i = 0; i < 23; i++) {
    M3L2BirkeLayer.push_back(0.);
  }
  for (unsigned int i = 0; i < 21; i++) {
    M1L3BirkeLayer.push_back(0.);
  }
  for (unsigned int i = 0; i < 21; i++) {
    M2L3BirkeLayer.push_back(0.);
  }
  for (unsigned int i = 0; i < 21; i++) {
    M3L3BirkeLayer.push_back(0.);
  }
  for (unsigned int i = 0; i < 20; i++) {
    M1L4BirkeLayer.push_back(0.);
  }
  for (unsigned int i = 0; i < 20; i++) {
    M2L4BirkeLayer.push_back(0.);
  }
  for (unsigned int i = 0; i < 20; i++) {
    M3L4BirkeLayer.push_back(0.);
  }

#ifdef ATLHECTB_LEAKANALYSIS
  SpectrumAnalyzer::GetInstance()->ClearEventFields();
#endif
}

void ATLHECTBEventAction::EndOfEventAction(const G4Event*)
{
  // Access Event random seeds
  //
  // auto rndseed = G4RunManager::GetRunManager()->GetRandomNumberStatusForThisEvent();

  // Accumulate statistics
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

#ifdef ATLHECTB_LEAKANALYSIS
  SpectrumAnalyzer::GetInstance()->FillEventFields();
#endif
}

//**************************************************
