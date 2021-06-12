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
        //const ATLHECTBDetectorConstruction* detConstruction,
        ATLHECTBEventAction* eventAction )
    : G4UserSteppingAction(),
      //fDetConstruction( detConstruction ),
      fEventAction( eventAction ) {
        
    fSCalculator = ATLHECTBSignalCalculator::Instance();
}

//Definition of deconstructor
//
ATLHECTBSteppingAction::~ATLHECTBSteppingAction(){}

//Definition of UserSteppingAction() method
//
void ATLHECTBSteppingAction::UserSteppingAction(const G4Step* step){

    //Save primary particle PDGID and vertex kinetic energy
    //
    if ( step->GetTrack()->GetTrackID() == 1 &&
         step->GetTrack()->GetCurrentStepNumber() == 1 ){
        fEventAction->SavePDGID(
                step->GetTrack()->GetParticleDefinition()->GetPDGEncoding());
        fEventAction->Savevertexkenergy(step->GetTrack()->GetVertexKineticEnergy());
    }
   
    //Collect out of world leakage
    //
    if ( !step->GetTrack()->GetNextVolume() ){
        fEventAction->Addeleak( step->GetTrack()->GetKineticEnergy() ); 
    }

    //Collect energy deposited in test beam prototype
    //all volumes inside HEC have copy number => 0
    //
    G4int cpNo = step->GetPreStepPoint()->GetTouchable()->GetCopyNumber();
    G4String matName = step->GetPreStepPoint()->GetMaterial()->GetName();
    G4String volName = step->GetPreStepPoint()->GetTouchable()->GetVolume()->GetName();

    if ( cpNo > 0 || ( cpNo == 0 && volName != "World" ) ){
        fEventAction->Addedep( step->GetTotalEnergyDeposit() );
    }

    //Collect energy deposited in cryostat and World
    //cryostat columes have copy number < 0
    //World has copy number = 0
    //
    if ( cpNo < 0 || ( cpNo == 0 && volName == "World" ) ){
        fEventAction->Addecryostat( step->GetTotalEnergyDeposit() ); 
    }

    //Collect energy deposited and Birk energy deposited in lAr gaps
    //
    if ( matName == "G4_lAr" ){
        G4int modulecpNo = step->GetPreStepPoint()->GetTouchable()->GetCopyNumber(2);
        G4double stepl = step->GetStepLength()/10.; //cm
        G4double edep = step->GetTotalEnergyDeposit();
        if ( stepl > 0. && edep > 0. ){
            fEventAction->AddelAr( edep );
            if ( step->GetTrack()->GetGlobalTime()-14. <= 75. ){  // nanoseconds (ns)
                fEventAction->AddBirkelAr( 
                    fSCalculator->ApplyBirks( edep, stepl ) );
               
                //Fill by Layer
                //
                G4ThreeVector origin(0.,0.,0.);
                G4ThreeVector sliceorigin = step->GetPreStepPoint()->GetTouchable()->
                GetHistory()->GetTopTransform().Inverse().TransformPoint(origin);
                G4ThreeVector hitpos = step->GetPreStepPoint()->GetPosition();
                G4ThreeVector relhitpos = hitpos-sliceorigin;
                if ( cpNo < 8 ){ //layer 1
                    G4int index = 
                        fSCalculator->IndexL1( hitpos.getEta(), relhitpos.getX() );
                    fEventAction->AddL1BirkeLayer( modulecpNo, index, 
                        fSCalculator->ApplyBirks( edep, stepl ));
                }
                else if ( 8<=cpNo && cpNo<24 ){ //layer 2
                    G4int index = 
                        fSCalculator->IndexL2( hitpos.getEta(), relhitpos.getX() );
                    fEventAction->AddL2BirkeLayer( modulecpNo, index, 
                        fSCalculator->ApplyBirks( edep, stepl ));
                }
                else if ( 24<=cpNo && cpNo<32 ){ //layer 3
                    G4int index = 
                        fSCalculator->IndexL3( hitpos.getEta(), relhitpos.getX() );
                    fEventAction->AddL3BirkeLayer( modulecpNo, index, 
                        fSCalculator->ApplyBirks( edep, stepl ));
                }
                else if ( 32<=cpNo && cpNo<40 ){ //layer 4
                    G4int index = 
                        fSCalculator->IndexL4( hitpos.getEta(), relhitpos.getX() );
                    fEventAction->AddL4BirkeLayer( modulecpNo, index, 
                        fSCalculator->ApplyBirks( edep ,stepl ));
                }

            } //if global time
        } //if stepl and edep
    } //if G4_lAr

}

//**************************************************
