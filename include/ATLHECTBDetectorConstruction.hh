//**************************************************
// \file ATLHECTBDetectorConstruction.hh
// \brief: Definition of ATLTBHECDetectorConstruction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

// Prevent including header multiple times
//
#ifndef ATLHECTBDetectorConstruction_h
#  define ATLHECTBDetectorConstruction_h 1

// Includers from Geant4
#  include "G4VUserDetectorConstruction.hh"
#  include "globals.hh"

class G4VPhysicalVolume;

class ATLHECTBDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    ATLHECTBDetectorConstruction();
    virtual ~ATLHECTBDetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();

  private:
    G4VPhysicalVolume* DefineVolumes();
    G4bool fCheckOverlaps;  // option to check volumes overlaps
    G4bool fDumpGDMLgeo;  // option to dump GDML geometry description
};

#endif

//**************************************************
