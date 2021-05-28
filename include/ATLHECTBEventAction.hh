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

#include <vector>

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
        void Addecryostat( G4double stepecryostat );
        void AddelAr( G4double stepelAr );
        void AddBirkelAr( G4double stepBirkelAr );
        void AddBirkeSlice( G4double stepBirkeSlice, G4int sliceNo );
        std::vector<G4double>& GetBirkeSlice() { return BirkeSlice; };
    
    private:
        G4int PDGID;                         //primary particle PDGID
        G4double vertexkenergy;              //primary particle kenergy (MeV)
        G4double eleakage;                   //out of world leakage (MeV)
        G4double edep;                       //energy deposited in HEC (MeV)
        G4double ecryostat;                  //energy deposited in cryostat (MeV)
        G4double elAr;                       //energy deposited in lAr (MeV)
        G4double BirkelAr;                   //Birk corrected elAr (a.u.)
        std::vector<G4double> BirkeSlice;    //Birk corrected elAr per 40 slice (a.u.)

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

inline void ATLHECTBEventAction::Addecryostat( G4double stepecryostat ){
    ecryostat += stepecryostat;
}

inline void ATLHECTBEventAction::AddelAr( G4double stepelAr ){
    elAr += stepelAr;
}

inline void ATLHECTBEventAction::AddBirkelAr( G4double stepBirkelAr ){
    BirkelAr += stepBirkelAr;
}

inline void ATLHECTBEventAction::AddBirkeSlice( G4double stepBirkeSlice, G4int sliceNo ){
    BirkeSlice[sliceNo] += stepBirkeSlice;
}

#endif

//**************************************************
