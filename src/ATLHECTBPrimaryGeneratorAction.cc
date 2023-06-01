//**************************************************
// \file ATLHECTBPrimaryGeneratorAction.cc
// \brief: Implementation of ATLHECTBPrimaryGeneratorAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

// Includers from project files
//
#include "ATLHECTBPrimaryGeneratorAction.hh"

// Includers from Geant4
//
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

// Define constructor
//
ATLHECTBPrimaryGeneratorAction::ATLHECTBPrimaryGeneratorAction()
  : G4VUserPrimaryGeneratorAction(), fParticleGun(nullptr)
{
  fParticleGun = new G4ParticleGun(1);  // set primary particle(s) to 1

  // default particle gun parameters (can be changed via UI)
  //
  auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("e-");
  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
  fParticleGun->SetParticleEnergy(1. * GeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., 0.));
}

// Define deconstructor
//
ATLHECTBPrimaryGeneratorAction::~ATLHECTBPrimaryGeneratorAction()
{
  delete fParticleGun;
}

// Define GeneratePrimaries() method
//
void ATLHECTBPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  fParticleGun->GeneratePrimaryVertex(event);
}

//**************************************************
