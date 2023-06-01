//**************************************************
// \file ATLHECTBSignalCalculator.cc
// \brief: Implementation of ATLHECTBSignalCalculator
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 11 May 2021
//**************************************************

// Includers from project files
//
#include "ATLHECTBSignalCalculator.hh"

// Includers from Geant4
//
#include "globals.hh"

G4ThreadLocal ATLHECTBSignalCalculator* ATLHECTBSignalCalculator::instance = 0;

// Define (private) constructor (singleton)
//
ATLHECTBSignalCalculator::ATLHECTBSignalCalculator() {}

// Define Instance() method
//
ATLHECTBSignalCalculator* ATLHECTBSignalCalculator::Instance()
{
  if (instance == 0) {
    instance = new ATLHECTBSignalCalculator;
  }
  return ATLHECTBSignalCalculator::instance;
}

// Define ApplyBirks() method
//
G4double ATLHECTBSignalCalculator::ApplyBirks(const G4double& de, const G4double& steplength)
{
  //--------------------------------------------------
  // Briks Law for LAr as implemented in ATHENA, from
  // HECTB-mysql_server/LArCalorimeter/LArG4TB/
  // LArG4TBHEC/src/LArG4TBHECCalculator.cc (ln 86-100)
  //--------------------------------------------------

  const G4double Emin = 1.51;  //(MeV cm2)/g, minimal energy loss in LAr
  const G4double Ef = 10.0;  // kV/cm, electric field in LAr gaps
  const G4double k = 0.0486;  // kV/cm g/(MeV cm2), paramter in Birks equation
  const G4double A = 1. + (k / Ef) * Emin;  // Parameter in Birks equation
  const G4double LArdensity = 1.396;  // g/cm3

  G4double Birks_dE_dx = de / steplength;  // MeV/cm
  G4double Birks_Corr;
  if (steplength < 1.e-5) {
    Birks_Corr = 1.0;  // no correction if dx<10^-5 cm
  }
  else {
    Birks_Corr = A / (1. + (k / Ef) * (1 / LArdensity) * (Birks_dE_dx));

    G4double kHIP;
    // The experimental data is available only until dE/dX ~ 12000 MeV/cm
    //
    if (Birks_dE_dx > 12000.0) {
      Birks_dE_dx = 12000.0;
    }

    // No corrections for dE/dX <= 969 MeV/cm
    //
    kHIP = (Birks_dE_dx > 969.) ? 0.000754 * Birks_dE_dx + 0.2692 : 1.0;
    Birks_Corr *= kHIP;
  }

  return de * Birks_Corr;
}

// Define indexL1() method
//
G4int ATLHECTBSignalCalculator::IndexL1(const G4double& eta, const G4double& relX)
{
  G4int index;

  if (1.5 < eta && eta < 1.6) {
    if (relX > 0) {
      index = 0;
    }
    else {
      index = 1;
    }
  }
  else if (1.6 < eta && eta < 1.7) {
    if (relX > 0) {
      index = 2;
    }
    else {
      index = 3;
    }
  }
  else if (1.7 < eta && eta < 1.8) {
    if (relX > 0) {
      index = 4;
    }
    else {
      index = 5;
    }
  }
  else if (1.8 < eta && eta < 1.9) {
    if (relX > 0) {
      index = 6;
    }
    else {
      index = 7;
    }
  }
  else if (1.9 < eta && eta < 2.0) {
    if (relX > 0) {
      index = 8;
    }
    else {
      index = 9;
    }
  }
  else if (2.0 < eta && eta < 2.1) {
    if (relX > 0) {
      index = 10;
    }
    else {
      index = 11;
    }
  }
  else if (2.1 < eta && eta < 2.2) {
    if (relX > 0) {
      index = 12;
    }
    else {
      index = 13;
    }
  }
  else if (2.2 < eta && eta < 2.3) {
    if (relX > 0) {
      index = 14;
    }
    else {
      index = 15;
    }
  }
  else if (2.3 < eta && eta < 2.4) {
    if (relX > 0) {
      index = 16;
    }
    else {
      index = 17;
    }
  }
  else if (2.4 < eta && eta < 2.5) {
    if (relX > 0) {
      index = 18;
    }
    else {
      index = 19;
    }
  }
  else if (2.5 < eta && eta < 2.7) {
    index = 20;
  }
  else if (2.7 < eta && eta < 2.9) {
    index = 21;
  }
  else if (2.9 < eta && eta < 3.1) {
    index = 22;
  }
  else if (3.1 < eta) {
    index = 23;
  }
  else {
    index = 0;
  }

  return index;
}

G4int ATLHECTBSignalCalculator::IndexL2(const G4double& eta, const G4double& relX)
{
  G4int index;

  if (1.5 < eta && eta < 1.6) {
    if (relX > 0) {
      index = 0;
    }
    else {
      index = 1;
    }
  }
  else if (1.6 < eta && eta < 1.7) {
    if (relX > 0) {
      index = 2;
    }
    else {
      index = 3;
    }
  }
  else if (1.7 < eta && eta < 1.8) {
    if (relX > 0) {
      index = 4;
    }
    else {
      index = 5;
    }
  }
  else if (1.8 < eta && eta < 1.9) {
    if (relX > 0) {
      index = 6;
    }
    else {
      index = 7;
    }
  }
  else if (1.9 < eta && eta < 2.0) {
    if (relX > 0) {
      index = 8;
    }
    else {
      index = 9;
    }
  }
  else if (2.0 < eta && eta < 2.1) {
    if (relX > 0) {
      index = 10;
    }
    else {
      index = 11;
    }
  }
  else if (2.1 < eta && eta < 2.2) {
    if (relX > 0) {
      index = 12;
    }
    else {
      index = 13;
    }
  }
  else if (2.2 < eta && eta < 2.3) {
    if (relX > 0) {
      index = 14;
    }
    else {
      index = 15;
    }
  }
  else if (2.3 < eta && eta < 2.4) {
    if (relX > 0) {
      index = 16;
    }
    else {
      index = 17;
    }
  }
  else if (2.4 < eta && eta < 2.5) {
    if (relX > 0) {
      index = 18;
    }
    else {
      index = 19;
    }
  }
  else if (2.5 < eta && eta < 2.7) {
    index = 20;
  }
  else if (2.7 < eta && eta < 2.9) {
    index = 21;
  }
  else if (2.9 < eta) {
    index = 22;
  }
  else {
    index = 0;
  }

  return index;
}

G4int ATLHECTBSignalCalculator::IndexL3(const G4double& eta, const G4double& relX)
{
  G4int index;

  if (1.6 < eta && eta < 1.7) {
    if (relX > 0) {
      index = 0;
    }
    else {
      index = 1;
    }
  }
  else if (1.7 < eta && eta < 1.8) {
    if (relX > 0) {
      index = 2;
    }
    else {
      index = 3;
    }
  }
  else if (1.8 < eta && eta < 1.9) {
    if (relX > 0) {
      index = 4;
    }
    else {
      index = 5;
    }
  }
  else if (1.9 < eta && eta < 2.0) {
    if (relX > 0) {
      index = 6;
    }
    else {
      index = 7;
    }
  }
  else if (2.0 < eta && eta < 2.1) {
    if (relX > 0) {
      index = 8;
    }
    else {
      index = 9;
    }
  }
  else if (2.1 < eta && eta < 2.2) {
    if (relX > 0) {
      index = 10;
    }
    else {
      index = 11;
    }
  }
  else if (2.2 < eta && eta < 2.3) {
    if (relX > 0) {
      index = 12;
    }
    else {
      index = 13;
    }
  }
  else if (2.3 < eta && eta < 2.4) {
    if (relX > 0) {
      index = 14;
    }
    else {
      index = 15;
    }
  }
  else if (2.4 < eta && eta < 2.5) {
    if (relX > 0) {
      index = 16;
    }
    else {
      index = 17;
    }
  }
  else if (2.5 < eta && eta < 2.7) {
    index = 18;
  }
  else if (2.7 < eta && eta < 2.9) {
    index = 19;
  }
  else if (2.9 < eta) {
    index = 20;
  }
  else {
    index = 0;
  }

  return index;
}

G4int ATLHECTBSignalCalculator::IndexL4(const G4double& eta, const G4double& relX)
{
  G4int index;

  if (1.7 < eta && eta < 1.8) {
    if (relX > 0) {
      index = 0;
    }
    else {
      index = 1;
    }
  }
  else if (1.8 < eta && eta < 1.9) {
    if (relX > 0) {
      index = 2;
    }
    else {
      index = 3;
    }
  }
  else if (1.9 < eta && eta < 2.0) {
    if (relX > 0) {
      index = 4;
    }
    else {
      index = 5;
    }
  }
  else if (2.0 < eta && eta < 2.1) {
    if (relX > 0) {
      index = 6;
    }
    else {
      index = 7;
    }
  }
  else if (2.1 < eta && eta < 2.2) {
    if (relX > 0) {
      index = 8;
    }
    else {
      index = 9;
    }
  }
  else if (2.2 < eta && eta < 2.3) {
    if (relX > 0) {
      index = 10;
    }
    else {
      index = 11;
    }
  }
  else if (2.3 < eta && eta < 2.4) {
    if (relX > 0) {
      index = 12;
    }
    else {
      index = 13;
    }
  }
  else if (2.4 < eta && eta < 2.5) {
    if (relX > 0) {
      index = 14;
    }
    else {
      index = 15;
    }
  }
  else if (2.5 < eta && eta < 2.7) {
    index = 16;
  }
  else if (2.7 < eta && eta < 2.9) {
    index = 17;
  }
  else if (2.9 < eta && eta < 3.1) {
    index = 18;
  }
  else if (3.1 < eta) {
    index = 19;
  }
  else {
    index = 0;
  }

  return index;
}

//**************************************************
