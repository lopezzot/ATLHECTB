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
    G4double zCoordinate[4],innerRadius[4],outerRadius[4]; //arrays for coordinates

    G4Tubs* solidDepths[7];          //array of 7 tubs
    G4LogicalVolume* logicDepth[7];  //array of 7 logical tubs
    G4VPhysicalVolume* physiDepth[7];//array of 7 physical tubs
    G4int gapNumber[7];              //array for gap numbers

    G4Tubs* solidSlice[3];           //array of 3 slices
    G4LogicalVolume* logicSlice[3];  //array of 3 logical slice
    G4VPhysicalVolume* physiSlice[3];//array of 3 physical slice



    


    //Module settings
    //
    G4double moduleNumber = 3;
    G4double moduleRinner1 = 37.2*cm;
    G4double moduleRinner2 = 37.2*cm;
    G4double moduleRouter = 203.*cm;
    G4double zStart = 427.7*cm;
    G4double copperPad = 0.003*cm;
    G4double gapSize = 8.5*mm;
    G4double betweenWheel = 40.5*mm;
    G4double moduleSize = 181.8*cm;

    G4double modulePhistart = 264.375*deg;
    G4double moduleDeltaPhi = 11.25*deg;
    zCoordinate[0] = 0.0*cm;
    zCoordinate[1] = 28.05*cm;
    zCoordinate[2] = 28.05*cm + 0.001*cm;
    zCoordinate[3] = 181.8*cm;
    innerRadius[0] = moduleRinner1;
    innerRadius[1] = moduleRinner1;
    innerRadius[2] = moduleRinner2;
    innerRadius[3] = moduleRinner2;

    G4int numberZplane = 4;
    G4int depthNumber = 7;
    G4double depthSize[7];
    G4double kaptonPosition[3];
    G4double kaptonWidth[3];

    for ( G4int index=0; index<numberZplane; index++) {outerRadius[index] = moduleRouter; }

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

    //(Sensitive) slice gaps
    //
    G4int sliceCopyNo = 0;
    G4int sliceNo;
    G4String depthName = "ATLHECTB::Depth";
    G4String sliceName = "ATLHECTB::Slice";
    G4double ModuleRinner = moduleRinner1;

    for ( sliceNo = 0; sliceNo<3; sliceNo++){
        if (sliceNo>0) ModuleRinner = moduleRinner2;
        solidSlice[sliceNo] = new G4Tubs(sliceName, ModuleRinner, moduleRouter,
                                         gapSize/2., modulePhistart, moduleDeltaPhi);
        logicSlice[sliceNo]= new G4LogicalVolume(solidSlice[sliceNo], lArMaterial, sliceName);
    }


















    //Place 7 depths in 1 HEC module
    //
    //G4double depthPositionZ = 0.;
    //for ( G4int iDepth = 0; iDepth<depthNumber; iDepth++ ){}
   








    //Visualization attributes
    //
    worldLV->SetVisAttributes( G4VisAttributes::GetInvisible() );

    auto HECVisAttributes = new G4VisAttributes();
    HECVisAttributes->SetForceWireframe( true );
    HECVisAttributes->SetColour( G4Colour::Red() );
    HECVisAttributes->SetLineWidth(5.0);
    logicHEC->SetVisAttributes( HECVisAttributes ); 

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
