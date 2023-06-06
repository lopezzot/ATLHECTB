//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// Construct hadron inelastic physics processes with FLUKA.CERN XS and models.
//
// Author: G.Hugo, 01 August 2022
//
// History: L. Pezzotti adaption of the FLUKAHadronInelasticPhysicsConstructor
// by G. Hugo to not include the HP treatment for neutrons. (June 6, 2023)
// ***************************************************************************
#ifdef G4_USE_FLUKA
#ifndef FLUKA_HADRON_INELASTIC_PHYSICS_HH
#define FLUKA_HADRON_INELASTIC_PHYSICS_HH 1

// G4
#include "G4VPhysicsConstructor.hh"

class FLUKAHadronInelasticPhysics final : public G4VPhysicsConstructor {

public:
  FLUKAHadronInelasticPhysics(G4int verbose = 1);

  virtual void ConstructParticle() override;
  virtual void ConstructProcess() override;

};


#endif // FLUKA_HADRON_INELASTIC_PHYSICS_HH
#endif // G4_USE_FLUKA
