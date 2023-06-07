// ***************************************************************************
//
// Construct hadron inelastic physics processes with FLUKA.CERN XS and models.
//
// Author: G.Hugo, 01 August 2022
//
// History: L. Pezzotti adaptation of the
//          FLUKAHadronInelasticPhysicsConstructor by G. Hugo
//          to not include the HP treatment for neutrons. (June 6, 2023)
// ***************************************************************************

#ifdef G4_USE_FLUKA
#  ifndef FLUKA_HADRON_INELASTIC_PHYSICS_HH
#    define FLUKA_HADRON_INELASTIC_PHYSICS_HH 1

// G4
#    include "G4VPhysicsConstructor.hh"

class FLUKAHadronInelasticPhysics final : public G4VPhysicsConstructor
{
  public:
    FLUKAHadronInelasticPhysics(G4int verbose = 1);

    virtual void ConstructParticle() override;
    virtual void ConstructProcess() override;
};

#  endif  // FLUKA_HADRON_INELASTIC_PHYSICS_HH
#endif  // G4_USE_FLUKA

// ***************************************************************************
