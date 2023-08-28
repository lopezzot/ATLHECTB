//**************************************************
// \file ATLHECTBSteppingAction.cc
// \brief: Implementation of ATLHECTBSteppingAction.cc
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

// Includers from project files
//
#include "ATLHECTBSteppingAction.hh"

#include "ATLHECTBDetectorConstruction.hh"
#include "ATLHECTBEventAction.hh"
#ifdef ATLHECTB_LEAKANALYSIS
#  include "SpectrumAnalyzer.hh"
#endif

// Includers from Geant4
//
#include "G4PhysicalConstants.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"

// Definition of constructor
//
ATLHECTBSteppingAction::ATLHECTBSteppingAction(
  // const ATLHECTBDetectorConstruction* detConstruction,
  ATLHECTBEventAction* eventAction)
  : G4UserSteppingAction(),
    // fDetConstruction( detConstruction ),
    fEventAction(eventAction)
{
  fSCalculator = ATLHECTBSignalCalculator::Instance();
}

// Definition of deconstructor
//
ATLHECTBSteppingAction::~ATLHECTBSteppingAction() {}

// Definition of UserSteppingAction() method
//
void ATLHECTBSteppingAction::UserSteppingAction(const G4Step* step)
{
  // Save primary particle PDGID and vertex kinetic energy
  //
  if (step->GetTrack()->GetTrackID() == 1 && step->GetTrack()->GetCurrentStepNumber() == 1) {
    fEventAction->SavePDGID(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding());
    fEventAction->Savevertexkenergy(step->GetTrack()->GetVertexKineticEnergy());
  }

  // Collect out of world leakage
  //
  if (!step->GetTrack()->GetNextVolume()) {
    fEventAction->Addeleak(step->GetTrack()->GetKineticEnergy());
#ifdef ATLHECTB_LEAKANALYSIS
    SpectrumAnalyzer::GetInstance()->Analyze(step);
#endif
  }

  // Collect energy deposited in test beam prototype
  // all volumes inside HEC have copy number => 0
  //
  G4int cpNo = step->GetPreStepPoint()->GetTouchable()->GetCopyNumber();
  G4String matName = step->GetPreStepPoint()->GetMaterial()->GetName();
  G4String volName = step->GetPreStepPoint()->GetTouchable()->GetVolume()->GetName();

  if (cpNo > 0 || (cpNo == 0 && volName != "World")) {
    fEventAction->Addedep(step->GetTotalEnergyDeposit());
  }

  // Collect energy deposited in cryostat and World
  // cryostat columes have copy number < 0
  // World has copy number = 0
  //
  if (cpNo < 0 || (cpNo == 0 && volName == "World")) {
    fEventAction->Addecryostat(step->GetTotalEnergyDeposit());
  }

  const G4double Phi0 = halfpi - (twopi / 32) * 1.5;  // starting Phi
  const G4double Phi1 = halfpi + (twopi / 32) * 1.5;  // ending Phi
  const G4double DeltaPhi = (Phi1 - Phi0) / 6.;  // 6 phi bins
  const G4double module1_halfPhi = Phi0 + DeltaPhi;
  const G4double module2_halfPhi = Phi0 + 3 * DeltaPhi;
  const G4double module3_halfPhi = Phi0 + 5 * DeltaPhi;

  // lambda to calculate wether a hit is on right or left side of module center
  //
  auto FindrelX = [module1_halfPhi, module2_halfPhi, module3_halfPhi](G4int modulecpNo,
                                                                      G4double hitPhi) {
    G4double module_halfPhi = 0.0;
    if (modulecpNo == 101)
      module_halfPhi = module1_halfPhi;
    else if (modulecpNo == 102)
      module_halfPhi = module2_halfPhi;
    else if (modulecpNo == 103)
      module_halfPhi = module3_halfPhi;
    else {
      G4ExceptionDescription msg;
      msg << "Wrong module cp No (!=101,102,103)";
      G4Exception("SteppingAction::UserSteppingAction()", "MyCode0004", FatalException, msg);
    }
    G4double relX = hitPhi > module_halfPhi ? -1.0 : 1.0;
    return relX;
  };

  // Collect energy deposited and Birk energy deposited in lAr gaps
  //
  if (matName == "G4_lAr" && volName == "ATLHECTB::Slice")
  {  // ATLHECTB::Slice check needed because ATLHECModule is G4_lAr too
    G4int modulecpNo = step->GetPreStepPoint()->GetTouchable()->GetCopyNumber(2);
    G4double stepl = step->GetStepLength() / 10.;  // cm
    G4double edep = step->GetTotalEnergyDeposit();  // MeV
    if (stepl > 0. && edep > 0.) {
      fEventAction->AddelAr(edep);
      fEventAction->AddBirkelAr(fSCalculator->ApplyBirks(edep, stepl));

      // Fill by Layer
      //
      G4ThreeVector hitpos = step->GetPreStepPoint()->GetPosition();
      G4double hitPhi = hitpos.getPhi();
      G4double relX = FindrelX(modulecpNo, hitPhi);

      if (cpNo < 8 && step->GetTrack()->GetGlobalTime() - 14. <= 75.) {  // layer 1
        G4int index = fSCalculator->IndexL1(hitpos.getEta(), relX);
        fEventAction->AddL1BirkeLayer(modulecpNo, index, fSCalculator->ApplyBirks(edep, stepl));
      }
      else if (8 <= cpNo && cpNo < 24 && step->GetTrack()->GetGlobalTime() - 15. <= 75.)
      {  // layer 2
        G4int index = fSCalculator->IndexL2(hitpos.getEta(), relX);
        fEventAction->AddL2BirkeLayer(modulecpNo, index, fSCalculator->ApplyBirks(edep, stepl));
      }
      else if (24 <= cpNo && cpNo < 32 && step->GetTrack()->GetGlobalTime() - 17. <= 75.)
      {  // layer 3
        G4int index = fSCalculator->IndexL3(hitpos.getEta(), relX);
        fEventAction->AddL3BirkeLayer(modulecpNo, index, fSCalculator->ApplyBirks(edep, stepl));
      }
      else if (32 <= cpNo && cpNo < 40 && step->GetTrack()->GetGlobalTime() - 18. <= 75.)
      {  // layer 4
        G4int index = fSCalculator->IndexL4(hitpos.getEta(), relX);
        fEventAction->AddL4BirkeLayer(modulecpNo, index, fSCalculator->ApplyBirks(edep, stepl));
      }

    }  // if stepl and edep
  }  // if G4_lAr
}

//**************************************************
