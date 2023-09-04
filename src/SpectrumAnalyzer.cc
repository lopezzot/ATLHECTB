//**************************************************
// \file SpectrumAnalyzer.cc
// \brief: Definition of SpectrumAnalyzer class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 28 August 2023
//**************************************************

#ifdef ATLHECTB_LEAKANALYSIS

// Includers from project files
//
#  include "SpectrumAnalyzer.hh"

// Includers from Geant4
//
#  include "G4Version.hh"
#  if G4VERSION_NUMBER < 1100
#    include "g4root.hh"
#  else
#    include "G4AnalysisManager.hh"
#  endif
#  include "G4ParticleTable.hh"
#  include "G4Step.hh"

// #define DEBUG

void SpectrumAnalyzer::CreateNtupleAndScorer(const G4String scName)
{
  auto AM = G4AnalysisManager::Instance();

  ntupleID = AM->CreateNtuple("Spectrum", "Spectrum");
  AM->CreateNtupleDColumn("neutronScore");
  AM->CreateNtupleDColumn("protonScore");
  AM->CreateNtupleDColumn("pionScore");
  AM->CreateNtupleDColumn("gammaScore");
  AM->CreateNtupleDColumn("electronScore");
  AM->CreateNtupleDColumn("othersScore");
  AM->FinishNtuple();

  // Define scorer type
  scorerName = scName;
  if (scorerName == "te") {
    scorer = GetTE;
    G4cout << "SpectrumAnalyzer scoring total energy" << G4endl;
  }
  else if (scorerName == "momentum") {
    scorer = GetMomentum;
    G4cout << "SpectrumAnalyzer scoring momentum" << G4endl;
  }
  else if (scorerName == "ke") {
    scorer = GetKE;
    G4cout << "SpectrumAnalyzer scoring kinetic energy" << G4endl;
  }
  else {
    scorer = GetTE;
    G4cout << "SpectrumAnalyzer scoring total energy" << G4endl;
  }  // default case
}

void SpectrumAnalyzer::FillEventFields() const
{
  auto AM = G4AnalysisManager::Instance();
  AM->FillNtupleDColumn(ntupleID, 0, neutronScore);
  AM->FillNtupleDColumn(ntupleID, 1, protonScore);
  AM->FillNtupleDColumn(ntupleID, 2, pionScore);
  AM->FillNtupleDColumn(ntupleID, 3, gammaScore);
  AM->FillNtupleDColumn(ntupleID, 4, electronScore);
  AM->FillNtupleDColumn(ntupleID, 5, othersScore);
  AM->AddNtupleRow(ntupleID);
}

void SpectrumAnalyzer::Analyze(const G4Step* step)
{
  auto PDGID = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  auto val = scorer(step);

  auto pTable = G4ParticleTable::GetParticleTable();
  const G4int neutronID = pTable->FindParticle("neutron")->GetPDGEncoding();
  const G4int antineutronID = pTable->FindParticle("anti_neutron")->GetPDGEncoding();
  const G4int protonID = pTable->FindParticle("proton")->GetPDGEncoding();
  const G4int antiprotonID = pTable->FindParticle("anti_proton")->GetPDGEncoding();
  const G4int pionplusID = pTable->FindParticle("pi+")->GetPDGEncoding();
  const G4int pionminusID = pTable->FindParticle("pi-")->GetPDGEncoding();
  const G4int electronID = pTable->FindParticle("e-")->GetPDGEncoding();
  const G4int positronID = pTable->FindParticle("e+")->GetPDGEncoding();
  const G4int gammaID = pTable->FindParticle("gamma")->GetPDGEncoding();

  if (PDGID == neutronID || PDGID == antineutronID) {
    neutronScore += val;
  }
  else if (PDGID == protonID || PDGID == antiprotonID) {
    protonScore += val;
  }
  else if (PDGID == pionplusID || PDGID == pionminusID) {
    pionScore += val;
  }
  else if (PDGID == gammaID) {
    gammaScore += val;
  }
  else if (PDGID == electronID || PDGID == positronID) {
    electronScore += val;
  }
  else {
    othersScore += val;
  }

#  ifdef DEBUG
  G4cout << "-->SpectrumAnalyzer::Analyze, scorer name " << scorerName << " " << PDGID << " "
         << step->GetTrack()->GetParticleDefinition()->GetParticleName() << " Total Energy "
         << GetTE(step) << " Momentum " << GetMomentum(step) << " Kinetic Energy " << GetKE(step)
         << G4endl;
#  endif
}

#endif  // ATLHECTB_LEAKANALYSIS

//**************************************************
