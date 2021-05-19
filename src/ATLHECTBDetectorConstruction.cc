//**************************************************
// \file ATLHECTBDetectorConstruction.cc
// \brief: Implementation of G4ATLTBHECDetectorConstruction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

//Includers from project files
//
#include "ATLHECTBDetectorConstruction.hh"

//Includers from Geant4
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Polycone.hh"
#include "G4Tubs.hh"

//Define constructor
//
ATLHECTBDetectorConstruction::ATLHECTBDetectorConstruction()
    : G4VUserDetectorConstruction(),
      fCheckOverlaps(true) {}

//Define deconstructor
//
ATLHECTBDetectorConstruction::~ATLHECTBDetectorConstruction() {}

//Define Construct() method
//
G4VPhysicalVolume* ATLHECTBDetectorConstruction::Construct(){
    return DefineVolumes();
}

//Define DefineVolumes() method
//
G4VPhysicalVolume* ATLHECTBDetectorConstruction::DefineVolumes(){

    //--------------------------------------------------
    //Define Elements, Mixtures and Materials
    //--------------------------------------------------

    //Getting materials from NIST (note: ATLAS defines materials from scratch)
    //
    auto nistManager = G4NistManager::Instance();
    auto FeMaterial = nistManager->FindOrBuildMaterial("G4_Fe");         //iron
    auto CuMaterial = nistManager->FindOrBuildMaterial("G4_Cu");         //copper
    auto lArMaterial = nistManager->FindOrBuildMaterial("G4_lAr");       //liquid Argon
    auto KaptonMaterial = nistManager->FindOrBuildMaterial("G4_KAPTON"); //KAPTON
    auto AirMaterial = nistManager->FindOrBuildMaterial("G4_AIR");       //air


    //World Construction
    //
    auto worldS = new G4Box("World", 10*m, 10*m, 10*m);
    auto worldLV = new G4LogicalVolume(worldS, AirMaterial, "World");
    auto worldPV = new G4PVPlacement(0,                      //no rotation
                                     G4ThreeVector(),        //at (0,0,0)
                                     worldLV,                //its LV
                                     "World",                //its name
                                     0,                      //its mother volume
                                     false,                  //no boolean operation
                                     0,                      //copynumber
                                     fCheckOverlaps);        //checking overlaps
   
    //--------------------------------------------------
    //Define ATLAS HEC TB geometry
    //--------------------------------------------------
    
    //Instantiate pointers to solid, logic and physic volumes
    //
    G4Polycone* solidHEC;            //solid HEC
    G4LogicalVolume* logicHEC;       //logical HEC
    G4VPhysicalVolume* physiHEC;     //physical HEC

    G4Polycone* solidModule;         //solid Module
    G4LogicalVolume* logicModule;    //logical Module
    G4VPhysicalVolume* physiModule;  //physical Module

    G4Tubs* solidDepths[7];          //array of 7 tubs
    G4LogicalVolume* logicDepth[7];  //array of 7 logical tubs
    G4VPhysicalVolume* physiDepth[7];//array of 7 physical tubs

    G4Tubs* solidSlice[3];           //array of 3 slices
    G4LogicalVolume* logicSlice[3];  //array of 3 logical slice
    G4VPhysicalVolume* physiSlice[3];//array of 3 physical slice

    G4Tubs* solidEstBoard;          //solid EstBoard
    G4LogicalVolume* logicEstBoard;  //logical EstBoard
    G4VPhysicalVolume* physiEstBoard;//physical EstBoard

    G4Tubs* solidPadBoard;           //solid PadBoard
    G4LogicalVolume* logicPadBoard;  //logic PadBoard
    G4VPhysicalVolume* physiPadBoard;//physical PadBoard
        
    G4Tubs* solidTieRod[2];          //array of two TieRod
    G4LogicalVolume* logicTieRod[2]; //array of two logical TieRod
    G4VPhysicalVolume* physiTieRod[2];//array of two physical TieRod

    G4Tubs* solidTieRodZone[2];      //for dead zone around TieRod, two TieRodZone
    G4LogicalVolume* logicTieRodZone[2];//array of two logical TieRodZone
    G4VPhysicalVolume* physiTieRodZone[2];//array of two physical TieRodZone

    G4Tubs* solidAbsorberTieRod[2]; //for TieRod in absorber, two AbsorberTieRod
    G4LogicalVolume* logicAbsorberTieRod[2];//array of two logical AbsorberTieRod
    G4VPhysicalVolume* physiAbsorberTieRod[2];//array of two physical AbsorberTieRod

    G4Tubs* solidAbsorber[3];        //array of three solid Absorber
    G4LogicalVolume* logicAbsorber[3];//array of three logical Absorber
    G4VPhysicalVolume* physiAbsorber[3];//array of three physical Absorber

    G4Tubs* solidFirstAbsorber;      //solid of FirstAbsorber
    G4LogicalVolume* logicFirstAbsorber;//logical of FirstAbsorber
    G4VPhysicalVolume* physiFirstAbsorber;//pysucal of FirstAbsorber


    //HEC geo parameters (see README.md for atlas-mysql parameter extraction)
    //
    G4int numberZplane = 4;
    G4int depthNumber = 7;

    G4double moduleNumber = 1;          //three modules for test-beam geometry
    G4double moduleRinner1 = 37.2*cm;   //LArHECmoduleRinner1, blrmn
    G4double moduleRinner2 = 47.5*cm;   //LArHECmoduleRinner2, blrmn
    G4double moduleRouter = 203.*cm;    //LArHECmoduleRouter, blrmx
    G4double zStart = 427.7*cm;         //LArHECzStart, zstart
    G4double copperPad = 0.003*cm;      //LArHECcopperPad, copper
    G4double gapSize = 8.5*mm;          //LArHECgapSize, larg
    G4double betweenWheel = 40.5*mm;    //LArHECbetweenWhell, gapwhl
    G4double moduleSize = 181.8*cm;     //module size

    //LArHECdepthZ, bldpth
    G4double depthSize[7] = {28.05*cm, 26.8*cm, 26.8*cm, 25.9*cm, 23.4*cm, 23.4*cm, 23.4*cm};
    //LArHECfirstAbsorber, plate0
    G4double firstAbsorber[7] = {1.25*cm, 0*cm, 0*cm, 2.5*cm, 0*cm, 0*cm, 0*cm};
    //LArHECgapNumber, blmod
    G4int gapNumber[7] = {8, 8, 8, 4, 4, 4, 4};
    //LArHECkaptonPosition, kptpos
    G4double kaptonPosition[3] = {0.204175*cm, 0.425*cm, 0.645825*cm};
    //LArHECkaptonWidth, dptwid
    G4double kaptonWidth[3] = {0.014*cm, 0.033*cm, 0.0333*cm};
    //LArHECrodPosX, rodposx
    G4double tieRodPositionX[4] = {0.*cm, 5.5*cm, 7.5*cm, 9.5*cm};
    //LArHECrodPosY, rodposr
    G4double tieRodPositionY[4] = {57.5*cm, 112.*cm, 155.*cm, 188.*cm};
    //LArHECRodDim, roddim
    G4double tieRodDiameter[2] = {1.2*cm, 1.6*cm};
    //LArHECSpcDim, spcdim
    G4double spacerDiameter[2] = {1.7*cm, 2.3*cm};

    G4double distance = zStart + depthSize[0]/2.0;
    G4double absorberZ1 = 2.5*cm;                  //LArHECplateWidth0, plate_0
    G4double absorberZ2 = 5.0*cm;                  //LArHECplateWidth1, plate_1

    G4double modulePhistart = 264.375*deg;         //(270.-11.25/2)*deg  
    G4double moduleDeltaPhi = 11.25*deg;

    G4double zCoordinate[4] = {0.0*cm, depthSize[0], depthSize[0]+0.001*cm, 181.8*cm};
    G4double innerRadius[4] = {moduleRinner1, moduleRinner1, moduleRinner2, moduleRinner2};
    G4double outerRadius[4] = {moduleRouter, moduleRouter, moduleRouter, moduleRouter};  

    //HEC construction
    //
    solidHEC = new G4Polycone("ATLHEC", 0., moduleNumber*moduleDeltaPhi,
                              numberZplane, zCoordinate, innerRadius, outerRadius);

    logicHEC = new G4LogicalVolume(solidHEC, lArMaterial, "ATLHEC");

    physiHEC = new G4PVPlacement(0, 
                                 G4ThreeVector(0.,0.,0.),
                                 logicHEC,
                                 "ATLHEC",
                                 worldLV,
                                 false,
                                 1,
                                 fCheckOverlaps);

    //Module construction
    //
    solidModule = new G4Polycone("ATLHECModule", modulePhistart, moduleDeltaPhi, 
                                 numberZplane, zCoordinate, innerRadius, outerRadius);

    logicModule = new G4LogicalVolume(solidModule, lArMaterial, "ATLHECModule");

    G4RotationMatrix moduleRotation;
    moduleRotation.rotateZ(-modulePhistart);

    for ( G4int iModule=0; iModule<moduleNumber; iModule++){
        physiModule = new G4PVPlacement(G4Transform3D(moduleRotation, G4ThreeVector()),
                                        logicModule, 
                                        "ATLHECModule",
                                        logicHEC,
                                        false,
                                        iModule,
                                        fCheckOverlaps); 
        moduleRotation.rotateZ(moduleDeltaPhi);
    }    

    //(Sensitive) slice gaps (solid and logic)
    //
    G4int sliceCopyNo = 0;
    G4int sliceNo;
    G4String depthName = "ATLHECTB::Depth";
    G4String sliceName = "ATLHECTB::Slice";
    G4double moduleRinner = moduleRinner1;

    for ( sliceNo = 0; sliceNo<3; sliceNo++ ){
        if (sliceNo>0) moduleRinner = moduleRinner2;
        solidSlice[sliceNo] = new G4Tubs(sliceName, moduleRinner, moduleRouter,
                                         gapSize/2., modulePhistart, moduleDeltaPhi);
        logicSlice[sliceNo]= new G4LogicalVolume(solidSlice[sliceNo], lArMaterial, sliceName);
    }






    //Place 7 depths in 1 HEC Module
    //
    G4double depthPositionZ = 0;
    for (G4int indexDepth = 0; indexDepth<depthNumber; indexDepth++ ){
        
        depthPositionZ += depthSize[indexDepth]/2.;
        if ( indexDepth == 1 ) depthPositionZ += 0.001*cm;
        moduleRinner = moduleRinner2;
        if ( indexDepth == 0 ) moduleRinner = moduleRinner1;

        G4double absorberSize = absorberZ2;
        if (indexDepth < 3 ) absorberSize = absorberZ1;
        G4double absorberPositionZ = firstAbsorber[indexDepth]+gapSize+absorberSize/2.0-depthSize[indexDepth]/2.0;
        
        solidDepths[indexDepth] = new G4Tubs( depthName, moduleRinner, moduleRouter,
                                             depthSize[indexDepth]/2., modulePhistart,
                                             moduleDeltaPhi );
        logicDepth[indexDepth] = new G4LogicalVolume( solidDepths[indexDepth], 
                                                      lArMaterial,
                                                      depthName);
        physiDepth[indexDepth] = new G4PVPlacement(0, 
                                                    G4ThreeVector(0.,0.,depthPositionZ),
                                                    depthName,
                                                    logicDepth[indexDepth],
                                                    physiModule,
                                                    fCheckOverlaps,
                                                    indexDepth);
        depthPositionZ += depthSize[indexDepth]/2.0; 
    
    
    
    
    }


















   






























    //Visualization attributes
    //
    worldLV->SetVisAttributes( G4VisAttributes::GetInvisible() );

    logicHEC->SetVisAttributes( G4VisAttributes::GetInvisible() );
    //auto HECVisAttributes = new G4VisAttributes();
    //HECVisAttributes->SetForceWireframe( true );
    //HECVisAttributes->SetColour( G4Colour::Red() );
    //HECVisAttributes->SetLineWidth(5.0);
    //logicHEC->SetVisAttributes( HECVisAttributes ); 

    auto ModuleVisAttributes = new G4VisAttributes();
    ModuleVisAttributes->SetForceWireframe( true );
    ModuleVisAttributes->SetLineWidth(5.0);
    ModuleVisAttributes->SetColour( G4Colour::Blue() );
    ModuleVisAttributes->SetForceAuxEdgeVisible( true );
    logicModule->SetVisAttributes( ModuleVisAttributes );

    G4cout<<"--->ATLHECTB geometry built<---"<<G4endl;

    //Return physical world
    //
    return worldPV;

}

//**************************************************
