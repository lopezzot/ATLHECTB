//**************************************************
// \file G4_CernFLUKAHadronInelastic_FTFP_BERT.hh
// \brief: Definition
// G4_CernFLUKAHadronInelastic_FTFP_BERT.hh class
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
#  ifndef G4_CernFLUKAHadronInelastic_FTFP_BERT_h
#    define G4_CernFLUKAHadronInelastic_FTFP_BERT_h 1

#    include "G4VModularPhysicsList.hh"
#    include "globals.hh"

#    include <CLHEP/Units/SystemOfUnits.h>

class G4_CernFLUKAHadronInelastic_FTFP_BERT final : public G4VModularPhysicsList
{
  public:
    G4_CernFLUKAHadronInelastic_FTFP_BERT(G4int ver = 1);
    virtual ~G4_CernFLUKAHadronInelastic_FTFP_BERT() = default;

    G4_CernFLUKAHadronInelastic_FTFP_BERT(const G4_CernFLUKAHadronInelastic_FTFP_BERT&) = delete;
    G4_CernFLUKAHadronInelastic_FTFP_BERT&
    operator=(const G4_CernFLUKAHadronInelastic_FTFP_BERT&) = delete;
};

#  endif  // G4_CernFLUKAHadronInelastic_FTFP_BERT_h
#endif  // G4_USE_FLUKA

//**************************************************
