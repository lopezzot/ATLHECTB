//**************************************************
// \file ATLHECTBPrimaryGeneratorAction.hh
// \brief: Definition of ATLHECTBPrimaryGeneratorAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

// Prevent including header multiple times
//
#ifndef ATLHECTBPrimaryGeneratorAction_h
#  define ATLHECTBPrimaryGeneratorAction_h 1

// Includers from Geant4
//
#  include "G4VUserPrimaryGeneratorAction.hh"
#  include "globals.hh"

class G4ParticleGun;
class G4Event;

class ATLHECTBPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    ATLHECTBPrimaryGeneratorAction();
    virtual ~ATLHECTBPrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* event);

    void SetRandomFlag(G4bool value);  // to check what this does

  private:
    G4ParticleGun* fParticleGun;
};

#endif

//**************************************************
