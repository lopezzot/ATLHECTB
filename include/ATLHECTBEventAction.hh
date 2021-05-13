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
};

#endif

//**************************************************
