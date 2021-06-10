//**************************************************
// \file ATLHECTBSignalCalculator.cc
// \brief: Implementation of ATLHECTBSignalCalculator class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

//Includers from project files
//
#include "ATLHECTBSignalCalculator.hh"

//Includers from Geant4
//
#include "globals.hh"

ATLHECTBSignalCalculator* ATLHECTBSignalCalculator::instance = 0;

//Define (private) constructor (singleton)
//
ATLHECTBSignalCalculator::ATLHECTBSignalCalculator(){}

//Define Instance() method
//
ATLHECTBSignalCalculator* ATLHECTBSignalCalculator::Instance(){ 
    if ( instance==0 ){
        instance = new ATLHECTBSignalCalculator;
    }
    return ATLHECTBSignalCalculator::instance;
}

//Define ApplyBirks() method
//
G4double ATLHECTBSignalCalculator::ApplyBirks( const G4double& de, const G4double& steplength ){

            const G4double Emin = 1.51;        //(MeV cm2)/g, minimal energy loss in LAr
            const G4double Ef = 10.0;           //kV/cm, electric field in LAr gaps  
            const G4double k = 0.0486;         //kV/cm g/(MeV cm2), paramter in Birks equation
            const G4double A = 1.+(k/Ef)*Emin; //Parameter in Birks equation
            const G4double LArdensity = 1.396; //g/cm3

            G4double satde = de*A/( 1.+ (k/Ef)*(1/LArdensity)*(de/steplength) ); 
    
            return satde;
}

//Define index L1() method
//
G4int ATLHECTBSignalCalculator::IndexL1( const G4double& eta, const G4double& relX ){
    G4int index;

    if ( 1.5<eta && eta<1.6 ) { 
        if ( relX > 0 ) {index = 0;}
        else            {index = 1;}}
    if ( 1.6<eta && eta<1.7 ) {
        if (relX > 0 ) {index = 2;}
        else           {index = 3;}}
    if (1.7<eta && eta<1.8 ) {
        if (relX > 0 ) {index = 4;}
        else           {index = 5;}}
    if (1.8<eta && eta<1.9) {
        if (relX > 0) {index = 6;}
        else          {index = 7;}}
    if (1.9<eta && eta<2.0) {
        if (relX > 0) {index = 8;}
        else          {index = 9;}}
    if (2.0<eta && eta<2.1) {
        if (relX > 0) {index = 10;}
        else          {index = 11;}}
    if (2.1<eta && eta<2.2) {
        if (relX > 0) {index = 12;}
        else          {index = 13;}}
    if (2.2<eta && eta<2.3) {
        if (relX > 0) {index = 14;}
        else          {index = 15;}}
    if (2.3<eta && eta<2.4) {
        if (relX > 0) {index = 16;}
        else          {index = 17;}}
    if (2.4<eta && eta<2.5) {
        if (relX > 0) {index = 18;}
        else          {index = 19;}}
    if (2.5<eta && eta<2.7)  {index = 20;}
    if (2.7<eta && eta<2.9)  {index = 21;}
    if (2.9<eta && eta<3.0)  {index = 22;}
    if (3.0<eta    )  {index = 23;}

    return index;
}

G4int ATLHECTBSignalCalculator::IndexL2( const G4double& eta, const G4double& relX ){
    G4int index;

    if ( 1.5<eta && eta<1.55 ) { 
        if ( relX > 0 ) {index = 0;}
        else            {index = 1;}}
    if ( 1.55<eta && eta<1.65 ) {
        if (relX > 0 ) {index = 2;}
        else           {index = 3;}}
    if (1.65<eta && eta<1.75 ) {
        if (relX > 0 ) {index = 4;}
        else           {index = 5;}}
    if (1.75<eta && eta<1.85) {
        if (relX > 0) {index = 6;}
        else          {index = 7;}}
    if (1.85<eta && eta<1.95) {
        if (relX > 0) {index = 8;}
        else          {index = 9;}}
    if (1.95<eta && eta<2.05) {
        if (relX > 0) {index = 10;}
        else          {index = 11;}}
    if (2.05<eta && eta<2.15) {
        if (relX > 0) {index = 12;}
        else          {index = 13;}}
    if (2.15<eta && eta<2.25) {
        if (relX > 0) {index = 14;}
        else          {index = 15;}}
    if (2.25<eta && eta<2.35) {
        if (relX > 0) {index = 16;}
        else          {index = 17;}}
    if (2.35<eta && eta<2.45) {
        if (relX > 0) {index = 18;}
        else          {index = 19;}}
    if (2.45<eta && eta<2.7)  {index = 20;}
    if (2.7<eta && eta<2.9)  {index = 21;}
    if (2.9<eta)  {index = 22;}

    return index;
}

G4int ATLHECTBSignalCalculator::IndexL3( const G4double& eta, const G4double& relX ){
    G4int index;

    if ( 1.5<eta && eta<1.55 ) { 
        if ( relX > 0 ) {index = 0;}
        else            {index = 1;}}
    if ( 1.55<eta && eta<1.65 ) {
        if (relX > 0 ) {index = 2;}
        else           {index = 3;}}
    if (1.65<eta && eta<1.75 ) {
        if (relX > 0 ) {index = 4;}
        else           {index = 5;}}
    if (1.75<eta && eta<1.85) {
        if (relX > 0) {index = 6;}
        else          {index = 7;}}
    if (1.85<eta && eta<1.95) {
        if (relX > 0) {index = 8;}
        else          {index = 9;}}
    if (1.95<eta && eta<2.05) {
        if (relX > 0) {index = 10;}
        else          {index = 11;}}
    if (2.05<eta && eta<2.15) {
        if (relX > 0) {index = 12;}
        else          {index = 13;}}
    if (2.15<eta && eta<2.25) {
        if (relX > 0) {index = 14;}
        else          {index = 15;}}
    if (2.25<eta && eta<2.35) {
        if (relX > 0) {index = 16;}
        else          {index = 17;}}
    if (2.35<eta && eta<2.6) {index = 18; }
    if (2.6<eta && eta<2.8)  {index = 19; }
    if (2.8<eta )            {index = 20; }

    return index;
}

G4int ATLHECTBSignalCalculator::IndexL4( const G4double& eta, const G4double& relX ){
    G4int index;

    if ( 1.5<eta && eta<1.55 ) { 
        if ( relX > 0 ) {index = 0;}
        else            {index = 1;}}
    if ( 1.55<eta && eta<1.65 ) {
        if (relX > 0 ) {index = 2;}
        else           {index = 3;}}
    if (1.65<eta && eta<1.75 ) {
        if (relX > 0 ) {index = 4;}
        else           {index = 5;}}
    if (1.75<eta && eta<1.85) {
        if (relX > 0) {index = 6;}
        else          {index = 7;}}
    if (1.85<eta && eta<1.95) {
        if (relX > 0) {index = 8;}
        else          {index = 9;}}
    if (1.95<eta && eta<2.05) {
        if (relX > 0) {index = 10;}
        else          {index = 11;}}
    if (2.05<eta && eta<2.15) {
        if (relX > 0) {index = 12;}
        else          {index = 13;}}
    if (2.15<eta && eta<2.25) {
        if (relX > 0) {index = 14;}
        else          {index = 15;}}
    if (2.25<eta && eta<2.35) {index = 16; }
    if (2.35<eta && eta<2.6)  {index = 17; }
    if (2.6<eta && eta<2.8)   {index = 18; }
    if (2.8<eta )             {index = 19; }

    return index;
}
//**************************************************
