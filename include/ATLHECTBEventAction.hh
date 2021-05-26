//**************************************************
// \file ATLHECTBEventAction.hh
// \brief: Definition of ATLHECTBEventAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

//Prevent including header multiple times
//
#ifndef ATLHECTBEventAction_h
#define ATLHECTBEventAction_h 1

//Includers from Gean4
//
#include "G4UserEventAction.hh"
#include "globals.hh"

class ATLHECTBEventAction : public G4UserEventAction {
    public:
        ATLHECTBEventAction();
        virtual ~ATLHECTBEventAction();

        virtual void BeginOfEventAction( const G4Event* event );
        virtual void EndOfEventAction( const G4Event* event );

        void SavePDGID( G4int ID );
        void Savevertexkenergy( G4double kenergy );
        void Addeleak( G4double eleak );
        void Addedep( G4double stepedep );

    private:
        G4int PDGID;
        G4double vertexkenergy;
        G4double eleakage; 
        G4double edep;
};

inline void ATLHECTBEventAction::SavePDGID( G4int ID){
    PDGID = ID;
}

inline void ATLHECTBEventAction::Savevertexkenergy( G4double kenergy ){
    vertexkenergy = kenergy;
}

inline void ATLHECTBEventAction::Addeleak( G4double eleak ){
    eleakage += eleak;
}

inline void ATLHECTBEventAction::Addedep( G4double stepedep ){
    edep += stepedep;
}
#endif

//**************************************************
