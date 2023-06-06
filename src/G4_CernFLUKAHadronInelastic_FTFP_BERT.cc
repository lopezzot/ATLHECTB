//**************************************************
// \file G4_CernFLUKAHadronInelastic_FTFP_BERT.cc
// \brief: Implementation of
// G4_CernFLUKAHadronInelastic_FTFP_BERT class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 2 June 2023
//**************************************************

/*Note:
This class is a customization of the FTFP_BERT physics list
to use the HadronInelastic process of FLUKA via the
FLUKA interface included in geant4-11.1.ref05.
*/

#ifdef G4_USE_FLUKA

// Includers from project files
//
#  include "G4_CernFLUKAHadronInelastic_FTFP_BERT.hh"
#  include "FLUKAHadronInelasticPhysics.hh"

// Includers from Geant4
//
#  include "G4DecayPhysics.hh"
#  include "G4EmExtraPhysics.hh"
#  include "G4EmStandardPhysics.hh"
#  include "G4HadronElasticPhysics.hh"
#  include "G4IonPhysics.hh"
#  include "G4NeutronTrackingCut.hh"
#  include "G4StoppingPhysics.hh"
#  include "G4ios.hh"
#  include "globals.hh"

#  include <iomanip>
// #include "G4HadronPhysicsFTFP_BERT.hh" //replaced by FLUKAHadronInelasticPhysics.hh

// Includers from FLUKA interface
//
#  include "fluka_interface.hh"

G4_CernFLUKAHadronInelastic_FTFP_BERT::G4_CernFLUKAHadronInelastic_FTFP_BERT(G4int ver)
{
  if (ver > 0) {
    G4cout << "Geant4 Physics List simulation engine: FTFP_BERT PL customized with "
              "FLUKAHadronInelasticPhysicsConstructor"
           << G4endl;
    G4cout << G4endl;
  }

  defaultCutValue = 0.7 * CLHEP::mm;
  SetVerboseLevel(ver);

  // EM Physics
  RegisterPhysics(new G4EmStandardPhysics(ver));

  // Synchroton Radiation & GN Physics
  RegisterPhysics(new G4EmExtraPhysics(ver));

  // Decays
  RegisterPhysics(new G4DecayPhysics(ver));

  // Hadron Elastic scattering
  RegisterPhysics(new G4HadronElasticPhysics(ver));

  // Hadron Physics
  // RegisterPhysics(  new G4HadronPhysicsFTFP_BERT(ver));
  RegisterPhysics(new FLUKAHadronInelasticPhysics(ver));

  // Stopping Physics
  RegisterPhysics(new G4StoppingPhysics(ver));

  // Ion Physics
  RegisterPhysics(new G4IonPhysics(ver));

  // Neutron tracking cut
  RegisterPhysics(new G4NeutronTrackingCut(ver));

  // IMPORTANT: Initialize the FLUKA interface here.
  // Both activation switches should be set to TRUE to provide the most comprehensive results.
  // NB: COMPARISON WITH G4 DOES NOT SEEM MEANINGFUL
  // WHEN COALESCENCE IS ACTIVATED IN BOTH FLUKA AND G4.
  // Freedom to choose & see the effect of these switches is hence provided here.
  const G4bool activateCoalescence = true;
  const G4bool activateHeavyFragmentsEvaporation = true;
  fluka_interface::initialize(activateCoalescence, activateHeavyFragmentsEvaporation);
}

#endif  // G4_USE_FLUKA

//**************************************************
