//**************************************************
// \file ATLHECTBDetectorConstruction.cc
// \brief: Implementation of G4ATLTBHECDetectorConstruction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim) @lopezzot
// \start date: 11 May 2021
//**************************************************

// Includers from project files
//
#include "ATLHECTBDetectorConstruction.hh"

// Includers from Geant4
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4GDMLParser.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4Polycone.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"

// Define constructor
//
ATLHECTBDetectorConstruction::ATLHECTBDetectorConstruction()
  : G4VUserDetectorConstruction(), fCheckOverlaps(false), fDumpGDMLgeo(false)
{}

// Define deconstructor
//
ATLHECTBDetectorConstruction::~ATLHECTBDetectorConstruction() {}

// Define Construct() method
//
G4VPhysicalVolume* ATLHECTBDetectorConstruction::Construct()
{
  auto WorldPV = DefineVolumes();
  if (fDumpGDMLgeo) {
    G4GDMLParser Parser;
    Parser.Write("ATLHECTBgeo.gdml", WorldPV);
  }
  return WorldPV;
}

// Define DefineVolumes() method
//
G4VPhysicalVolume* ATLHECTBDetectorConstruction::DefineVolumes()
{
  //--------------------------------------------------
  // Define Elements, Mixtures and Materials
  //--------------------------------------------------

  // Getting materials from NIST (note: ATLAS defines materials from scratch)
  //
  // auto nistManager = G4NistManager::Instance();
  /*
  auto FeMaterial = nistManager->FindOrBuildMaterial("G4_Fe");           //iron
  auto CuMaterial = nistManager->FindOrBuildMaterial("G4_Cu");           //copper
  auto lArMaterial = nistManager->FindOrBuildMaterial("G4_lAr");         //liquid Argon
  auto KaptonMaterial = nistManager->FindOrBuildMaterial("G4_KAPTON");   //KAPTON
  */
  // auto AirMaterial = nistManager->FindOrBuildMaterial("G4_AIR");         //air
  // auto VacuumMaterial = nistManager->FindOrBuildMaterial("G4_Galactic"); //vacuum
  // Rohacell material
  // auto RohacellMaterial = new G4Material("Rohacell",
  //                                        6.18, 12.957*g/mole, 0.112*g/cm3);

  // Using materials as defined by ATLAS (HEC+Cryostat)
  //
  G4String name, symbol;
  G4double a, z, density;
  G4int ncomponents, natoms;

  a = 1.01 * g / mole;
  G4Element* elH = new G4Element(name = "Hydrogen", symbol = "H", z = 1., a);

  a = 12.01 * g / mole;
  G4Element* elC = new G4Element(name = "Carbon", symbol = "C", z = 6., a);

  a = 14.01 * g / mole;
  G4Element* elN = new G4Element(name = "Nitrogen", symbol = "N", z = 7., a);

  a = 16.00 * g / mole;
  G4Element* elO = new G4Element(name = "Oxygen", symbol = "O", z = 8., a);

  // a = 39.95*g/mole;
  // G4Element *elAr = new G4Element(name="Argon", symbol="Ar", z=18., a);

  a = 39.95 * g / mole;
  density = 1.396 * g / cm3;
  G4Material* lArMaterial = new G4Material(name = "G4_lAr", 18., a, density);

  a = 55.845 * g / mole;
  density = 7.87 * g / cm3;
  G4Material* FeMaterial = new G4Material(name = "G4_Fe", z = 26., a, density);

  a = 63.546 * g / mole;
  density = 8.960 * g / cm3;
  G4Material* CuMaterial = new G4Material(name = "G4_Cu", z = 29., a, density);

  // 11-Jan-2002 ML from accbgeo.age: the Kapton_E density is 1.46g/cm3
  // one assumes it is the same as for the Kapton_H -> C22 H10 O5 N2
  density = 1.46 * g / cm3;
  G4Material* KaptonMaterial = new G4Material(name = "G4_KAPTON", density, ncomponents = 4);
  KaptonMaterial->AddElement(elH, natoms = 10);
  KaptonMaterial->AddElement(elC, natoms = 22);
  KaptonMaterial->AddElement(elO, natoms = 5);
  KaptonMaterial->AddElement(elN, natoms = 2);
  // end of materials from ATLAS for HEC

  density = universe_mean_density;  // from G4PhysicalConstants.hh
  G4double pressure = 3.e-18 * pascal;
  G4double temperature = 2.73 * kelvin;
  G4double fractionmass;
  G4Material* VacuumMaterial = new G4Material(name = "G4_Galactic", z = 1., a = 1.01 * g / mole,
                                              density, kStateGas, temperature, pressure);

  a = 12.957 * g / mole;
  density = 0.112 * g / cm3;
  G4Material* RohacellMaterial = new G4Material(name = "Rohacell", z = 6.18, a, density);

  G4Material* AirMaterial =
    new G4Material(name = "G4_AIR", density = 1.290 * mg / cm3, ncomponents = 2);
  AirMaterial->AddElement(elN, fractionmass = 0.7);
  AirMaterial->AddElement(elO, fractionmass = 0.3);
  // end of materials from ATLAS for cryostat and world

  // World Construction
  //
  G4double bryr_x = 200.0 * cm;  // dimension of room with cryostat
  G4double bryr_y = 200.0 * cm;
  G4double bryr_z = 350.0 * cm;  // in ATLAS = 129.55*cm;
  auto solidWorld = new G4Box("World", 2. * bryr_x, 2 * bryr_y, 2 * bryr_z);
  auto logicWorld = new G4LogicalVolume(solidWorld, AirMaterial, "World");
  logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
  auto physiWorld = new G4PVPlacement(0,  // no rotation
                                      G4ThreeVector(),  // at (0,0,0)
                                      logicWorld,  // its LV
                                      "World",  // its name
                                      0,  // its mother volume
                                      false,  // no boolean operation
                                      0,  // copynumber
                                      fCheckOverlaps);  // checking overlaps

  //--------------------------------------------------
  // Define cryostat geometry
  //--------------------------------------------------

  // Cryostat geo parameters
  //
  G4double bcry_rwarm = 129.55 * cm;  // dimension of different walls of cryostat
  G4double bcry_rvac = 129.3 * cm;
  G4double bcry_rcold = 125.8 * cm;
  G4double bcry_rlar = 125.5 * cm;
  // G4double   bcry_dz = 100.0*cm;

  // G4double   bepo_Angle=0.0*degree; //position of HEC $ leakage det inside cryostat
  G4double bepo_x = 0.0 * cm;
  G4double bepo_y = bcry_rlar - 44.5 * cm;
  G4double bepo_z = 172. * cm;
  G4double bepo_tx = 180.0 * degree;
  G4double bepo_ty = 90.0 * degree;
  G4double bepo_tz = 90.0 * degree;
  G4double bepo_px = 270.0 * degree;
  G4double bepo_py = 0.0 * degree;
  G4double bepo_pz = 270.0 * degree;
  // G4double   bepo_Rtot = 204.0*cm;
  // G4double   bepo_Ztot = 182.0*cm;
  // G4double   bepo_Thick = 3.0*cm;
  // G4double   bepo_Shift = 2.0*cm;
  // G4double   bepo_Edge = 10.0*cm;
  // G4double bepo_Beta = 3.0*pi/32.0*degree;
  // G4double   bepo_Beta = 16.875*degree;

  // Initialize pointers for cryostat geometry
  //
  G4Tubs* brww_tub;
  G4LogicalVolume* brww_log;
  // G4VPhysicalVolume *brww_phys;

  G4Tubs* brvv_tub;
  G4LogicalVolume* brvv_log;
  // G4VPhysicalVolume *brvv_phys;

  G4Tubs* bryw_tub;
  G4LogicalVolume* bryw_log;
  // G4VPhysicalVolume *bryw_phys;

  G4Tubs* bryi_tub;
  G4LogicalVolume* bryi_log;
  // G4VPhysicalVolume *bryi_phys;

  // G4LogicalVolume* left_log;       //used by ATLAS for leakage detectors
  // G4VPhysicalVolume* left_phys;
  // G4Transform3D left_pos;

  // G4LogicalVolume* right_log;
  // G4VPhysicalVolume* right_phys;
  // G4Transform3D right_pos;

  // G4LogicalVolume* down_log;
  // G4VPhysicalVolume* down_phys;
  // G4Transform3D down_pos;

  // G4LogicalVolume* back_log;
  // G4VPhysicalVolume* back_phys;
  // G4Transform3D back_pos;

  // Warm cryostat wall
  //
  auto brwwVisAttributes = new G4VisAttributes();
  brwwVisAttributes->SetForceWireframe(true);
  brwwVisAttributes->SetLineWidth(6.0);
  brwwVisAttributes->SetColour(G4Colour::Grey());

  brww_tub = new G4Tubs("brww_tubw", 0.0 * cm, bcry_rwarm, bryr_y, 0.0 * degree, 360.0 * degree);
  brww_log = new G4LogicalVolume(brww_tub, FeMaterial, "brww_log");
  brww_log->SetVisAttributes(brwwVisAttributes);
  G4RotationMatrix cryostatrot;
  cryostatrot.rotateX(-90. * deg);
  cryostatrot.rotateZ(180. * deg);
  G4Transform3D cryostatpos = G4Transform3D(cryostatrot, G4ThreeVector(0., 1720. * mm, 5. * m));
  /*brww_phys =*/new G4PVPlacement(cryostatpos, brww_log, "brww_phys", logicWorld, false, -1);
  // Vacuum between cryostat walls
  //
  auto brvvVisAttributes = new G4VisAttributes();
  brvvVisAttributes->SetForceWireframe(true);
  // brvvVisAttributes->SetForceSolid( true );
  brvvVisAttributes->SetLineWidth(6.0);
  brvvVisAttributes->SetColour(G4Colour::Blue());

  brvv_tub = new G4Tubs("brvv_tub", 0.0 * cm, bcry_rvac, bryr_y, 0.0 * degree, 360. * degree);
  brvv_log = new G4LogicalVolume(brvv_tub, VacuumMaterial, "brvv_log");
  brvv_log->SetVisAttributes(brvvVisAttributes);

  /*brvv_phys =*/new G4PVPlacement(0, G4ThreeVector(), brvv_log, "brvv_phys", brww_log, false, -2);

  // Cold cryostat wall
  //
  auto brywVisAttributes = new G4VisAttributes();
  brywVisAttributes->SetForceWireframe(true);
  // brywVisAttributes->SetForceSolid( true );
  brywVisAttributes->SetLineWidth(6.0);
  brywVisAttributes->SetColour(G4Colour::Grey());

  bryw_tub = new G4Tubs("bryw_tub", 0.0 * cm, bcry_rcold, bryr_y, 0.0 * degree, 360.0 * degree);
  bryw_log = new G4LogicalVolume(bryw_tub, FeMaterial, "bryw_log");
  bryw_log->SetVisAttributes(brywVisAttributes);
  /*bryw_phys =*/new G4PVPlacement(0, G4ThreeVector(), bryw_log, "bryw_phys", brvv_log, false, -3);

  // Inside cryostat
  //
  auto bryiVisAttributes = new G4VisAttributes();
  bryiVisAttributes->SetForceWireframe(true);
  // bryiVisAttributes->SetForceSolid( true );
  bryiVisAttributes->SetLineWidth(6.0);
  bryiVisAttributes->SetColour(G4Colour::Cyan());

  bryi_tub = new G4Tubs("bryi_tub", 0.0 * cm, bcry_rlar, bryr_y, 0.0 * degree, 360.0 * degree);
  // bryi_log = new G4LogicalVolume(bryi_tub, AirMaterial, "bryi_log");
  bryi_log = new G4LogicalVolume(bryi_tub, RohacellMaterial, "bryi_log");
  bryi_log->SetVisAttributes(bryiVisAttributes);
  /*bryi_phys =*/new G4PVPlacement(0, G4ThreeVector(), bryi_log, "bryi_phys", bryw_log, false, -4);

  // Prepare for HEC placement within cryostat
  //
  G4RotationMatrix hecrot;
  hecrot.rotateAxes(
    G4ThreeVector(sin(bepo_tx) * cos(bepo_px), sin(bepo_tx) * sin(bepo_px), cos(bepo_tx)),
    G4ThreeVector(sin(bepo_ty) * cos(bepo_py), sin(bepo_ty) * sin(bepo_py), cos(bepo_ty)),
    G4ThreeVector(sin(bepo_tz) * cos(bepo_pz), sin(bepo_tz) * sin(bepo_pz), cos(bepo_tz)));
  hecrot.rotateY(3. * M_PI / 32);  // 3/2 * moduleDeltaPhi
  G4Transform3D hecpos = G4Transform3D(hecrot, G4ThreeVector(bepo_x, bepo_y, bepo_z));

  //--------------------------------------------------
  // Define ATLAS HEC TB geometry
  //--------------------------------------------------

  // Instantiate pointers to solid, logic and physic volumes
  //
  G4Polycone* solidHEC;  // solid HEC
  G4LogicalVolume* logicHEC;  // logical HEC
  // G4VPhysicalVolume* physiHEC;          //physical HEC

  G4Polycone* solidModule;  // solid Module
  G4LogicalVolume* logicModule;  // logical Module
  G4VPhysicalVolume* physiModule;  // physical Module

  G4Tubs* solidDepths[7];  // array of 7 tubs for Depths
  G4LogicalVolume* logicDepth[7];  // array of 7 logical tubs
  G4VPhysicalVolume* physiDepth[7];  // array of 7 physical tubs

  G4Tubs* solidSlice[3];  // array of 3 slices for Slice
  G4LogicalVolume* logicSlice[3];  // array of 3 logical slice
  // G4VPhysicalVolume* physiSlice[3];     //array of 3 physical slice

  G4Tubs* solidEstBoard;  // solid EstBoard
  G4LogicalVolume* logicEstBoard;  // logical EstBoard
  G4VPhysicalVolume* physiEstBoard;  // physical EstBoard

  G4Tubs* solidPadBoard;  // solid PadBoard
  G4LogicalVolume* logicPadBoard;  // logic PadBoard
  // G4VPhysicalVolume* physiPadBoard;     //physical PadBoard

  G4Tubs* solidTieRod[2];  // array of two TieRod
  G4LogicalVolume* logicTieRod[2];  // array of two logical TieRod
  // G4VPhysicalVolume* physiTieRod[2];    //array of two physical TieRod

  bool TieRodZone = false;  // true for including dead zones around tierods
  G4Tubs* solidTieRodZone[2];  // for dead zone on TieRod, two TieRodZone
  G4LogicalVolume* logicTieRodZone[2];  // array of two logical TieRodZone
  // G4VPhysicalVolume* physiTieRodZone[2];//array of two physical TieRodZone

  G4Tubs* solidAbsorberTieRod[2];  // for TieRod in absorber, two AbsorberTieRod
  G4LogicalVolume* logicAbsorberTieRod[2];  // array of two logical AbsorberTieRod
  // G4VPhysicalVolume* physiAbsorberTieRod[2];//array of two physical AbsorberTieRod

  G4Tubs* solidAbsorber[3];  // array of three solid Absorber
  G4LogicalVolume* logicAbsorber[3];  // array of three logical Absorber
  // G4VPhysicalVolume* physiAbsorber[3];  //array of three physical Absorber

  G4Tubs* solidFirstAbsorber;  // solid of FirstAbsorber
  G4LogicalVolume* logicFirstAbsorber;  // logical of FirstAbsorber
  // G4VPhysicalVolume* physiFirstAbsorber;//pysical of FirstAbsorber

  // HEC geo parameters (see README.md for atlas-mysql parameter extraction)
  //
  G4int numberZplane = 4;
  G4int depthNumber = 7;
  G4double moduleNumber = 3;  // three modules for test-beam geometry
                              //  32 for the whole ATLAS HEC geometry

  // alias in atl-det-construction, mysql name
  G4double moduleRinner1 = 37.2 * cm;  // LArHECmoduleRinner1, blrmn
  G4double moduleRinner2 = 47.5 * cm;  // LArHECmoduleRinner2, blrmn
  G4double moduleRouter = 203. * cm;  // LArHECmoduleRouter, blrmx
  // G4double zStart = 427.7*cm;       //LArHECzStart, zstart
  //  (used by atl for G4double distance,
  //   not used in the construction.
  //   Comment this value)
  G4double copperPad = 0.003 * cm;  // LArHECcopperPad, copper
  G4double gapSize = 8.5 * mm;  // LArHECgapSize, larg
  G4double betweenWheel = 40.5 * mm;  // LArHECbetweenWheel, gapwhl
  G4double moduleSize = 181.8 * cm;  // module size

  // LArHECdepthZ, bldpth
  G4double depthSize[7] = {28.05 * cm, 26.8 * cm, 26.8 * cm, 25.9 * cm,
                           23.4 * cm,  23.4 * cm, 23.4 * cm};
  // LArHECfirstAbsorber, plate0
  G4double firstAbsorber[7] = {1.25 * cm, 0 * cm, 0 * cm, 2.5 * cm, 0 * cm, 0 * cm, 0 * cm};
  // LArHECgapNumber, blmod
  G4int gapNumber[7] = {8, 8, 8, 4, 4, 4, 4};
  // LArHECkaptonPosition, kptpos
  G4double kaptonPosition[3] = {0.204175 * cm, 0.425 * cm, 0.645825 * cm};
  // LArHECkaptonWidth, kptwid
  G4double kaptonWidth[3] = {0.014 * cm, 0.0333 * cm, 0.014 * cm};
  // LArHECrodPosX, rodposx
  G4double tieRodPositionX[4] = {0. * cm, 5.5 * cm, 7.5 * cm, 9.5 * cm};
  // LArHECrodPosY, rodposr
  G4double tieRodPositionY[4] = {57.5 * cm, 112. * cm, 155. * cm, 188. * cm};
  // LArHECRodDim, roddim
  G4double tieRodDiameter[2] = {1.2 * cm, 1.6 * cm};
  // LArHECSpcDim, spcdim
  G4double spacerDiameter[2] = {1.7 * cm, 2.3 * cm};

  // G4double distance = zStart + depthSize[0]/2.0; //(not used by atl, comment it)
  G4double absorberZ1 = 2.5 * cm;  // LArHECplateWidth0, plate_0
  G4double absorberZ2 = 5.0 * cm;  // LArHECplateWidth1, plate_1

  G4double modulePhistart = 264.375 * deg;  //(270.-11.25/2)*deg
  G4double moduleDeltaPhi = 11.25 * deg;  // 360/32=11.25*deg

  G4double zCoordinate[4] = {0.0 * cm, depthSize[0], depthSize[0] + 0.001 * cm, moduleSize};
  G4double innerRadius[4] = {moduleRinner1, moduleRinner1, moduleRinner2, moduleRinner2};
  G4double outerRadius[4] = {moduleRouter, moduleRouter, moduleRouter, moduleRouter};

  // HEC construction
  //
  solidHEC = new G4Polycone("ATLHEC", 0., moduleNumber * moduleDeltaPhi, numberZplane, zCoordinate,
                            innerRadius, outerRadius);

  logicHEC = new G4LogicalVolume(solidHEC, lArMaterial, "ATLHEC");

  // auto HECVisAttributes = new G4VisAttributes();      //for image display
  // HECVisAttributes->SetForceWireframe( true );        //for image display
  // HECVisAttributes->SetLineWidth(12.0);               //for image display
  // HECVisAttributes->SetColour( G4Colour::Red() );     //for image display
  // logicHEC->SetVisAttributes( HECVisAttributes );     //for image display
  logicHEC->SetVisAttributes(G4VisAttributes::GetInvisible());

  /*physiHEC =*/new G4PVPlacement(hecpos, logicHEC, "ATLHEC", bryi_log, false, 0, fCheckOverlaps);

  // Module construction
  //
  solidModule = new G4Polycone("ATLHECModule", modulePhistart, moduleDeltaPhi, numberZplane,
                               zCoordinate, innerRadius, outerRadius);

  logicModule = new G4LogicalVolume(solidModule, lArMaterial, "ATLHECModule");
  // Caveat: in ATLAS Module material is lAr but I could put it to AirMaterial

  // auto ModuleVisAttributes = new G4VisAttributes();      //for image display
  // ModuleVisAttributes->SetForceWireframe( true );        //for image display
  // ModuleVisAttributes->SetLineWidth(12.0);               //for image display
  // ModuleVisAttributes->SetColour( G4Colour::Blue() );    //for image display
  // logicModule->SetVisAttributes( ModuleVisAttributes );  //for image display
  logicModule->SetVisAttributes(G4VisAttributes::GetInvisible());

  G4RotationMatrix moduleRotation;
  moduleRotation.rotateZ(-modulePhistart);

  for (G4int iModule = 0; iModule < moduleNumber; iModule++) {
    physiModule = new G4PVPlacement(G4Transform3D(moduleRotation, G4ThreeVector()), logicModule,
                                    "ATLHECModule", logicHEC, false,
                                    101 + iModule,  // 101,102,103
                                    fCheckOverlaps);
    moduleRotation.rotateZ(moduleDeltaPhi);
  }

  //(Sensitive) slice gaps (solid and logic)
  //
  G4double maxstepslice = gapSize / 2.;  // at least two steps per track in LAr
  auto StepLimit = new G4UserLimits();
  StepLimit->SetMaxAllowedStep(maxstepslice);

  G4int sliceCopyNo = 0;
  G4int sliceNo;
  G4String sliceName = "ATLHECTB::Slice";
  G4double moduleRinner = moduleRinner1;

  auto SliceVisAttributes = new G4VisAttributes();
  // SliceVisAttributes->SetForceWireframe( true );
  SliceVisAttributes->SetForceSolid(true);
  SliceVisAttributes->SetLineWidth(2.0);
  SliceVisAttributes->SetColour(G4Colour::Blue());

  for (sliceNo = 0; sliceNo < 3; sliceNo++) {
    if (sliceNo > 0) moduleRinner = moduleRinner2;
    solidSlice[sliceNo] = new G4Tubs(sliceName, moduleRinner, moduleRouter, gapSize / 2.,
                                     modulePhistart, moduleDeltaPhi);
    logicSlice[sliceNo] = new G4LogicalVolume(solidSlice[sliceNo], lArMaterial, sliceName);
    logicSlice[sliceNo]->SetUserLimits(StepLimit);
    logicSlice[sliceNo]->SetVisAttributes(SliceVisAttributes);
  }

  // Absorber
  //
  G4int absorberNo;
  G4String absorberName = "ATLASHECTB::Absorber";
  G4double absorberRinner1 = moduleRinner1 - 1.02 * cm;
  G4double absorberRinner2 = moduleRinner2 - 1.02 * cm;
  G4double absorberRouter = moduleRouter - 1.02 * cm;

  auto AbsorberVisAttributes = new G4VisAttributes();
  // AbsorberVisAttributes->SetForceWireframe( true );
  AbsorberVisAttributes->SetForceSolid(true);
  AbsorberVisAttributes->SetLineWidth(2.0);
  AbsorberVisAttributes->SetColour(G4Colour::Brown());

  solidAbsorber[0] = new G4Tubs(absorberName, absorberRinner1, absorberRouter, absorberZ1 / 2.,
                                modulePhistart, moduleDeltaPhi);
  solidAbsorber[1] = new G4Tubs(absorberName, absorberRinner2, absorberRouter, absorberZ1 / 2.,
                                modulePhistart, moduleDeltaPhi);
  solidAbsorber[2] = new G4Tubs(absorberName, absorberRinner2, absorberRouter, absorberZ2 / 2.,
                                modulePhistart, moduleDeltaPhi);
  for (absorberNo = 0; absorberNo < 3; absorberNo++) {
    logicAbsorber[absorberNo] =
      new G4LogicalVolume(solidAbsorber[absorberNo], CuMaterial, absorberName);
    logicAbsorber[absorberNo]->SetVisAttributes(AbsorberVisAttributes);
  }

  // First Absorber
  //
  auto FirstAbsorberVisAttributes = new G4VisAttributes();
  // FirstAbsorberVisAttributes->SetForceWireframe( true );
  FirstAbsorberVisAttributes->SetForceSolid(true);
  FirstAbsorberVisAttributes->SetLineWidth(2.0);
  FirstAbsorberVisAttributes->SetColour(G4Colour::Brown());

  G4String firstAbsorbername = "ATLHECTBFirstAbsorber";
  solidFirstAbsorber =
    new G4Tubs(firstAbsorbername, moduleRinner1 - 1.02 * cm, moduleRouter - 1.02 * cm,
               firstAbsorber[0] / 2., modulePhistart, moduleDeltaPhi);

  logicFirstAbsorber = new G4LogicalVolume(solidFirstAbsorber, CuMaterial, firstAbsorbername);
  logicFirstAbsorber->SetVisAttributes(FirstAbsorberVisAttributes);

  // Place 7 depths in 1 HEC Module
  //
  auto DepthVisAttributes = new G4VisAttributes();  // for image display
  DepthVisAttributes->SetForceWireframe(true);  // for image display
  DepthVisAttributes->SetLineWidth(5.0);  // for image display
  DepthVisAttributes->SetColour(G4Colour::Grey());  // for image display

  G4String depthName = "ATLHECTB::Depth";
  G4double depthPositionZ = 0;
  G4double absorberPosY = -1.02 * cm;

  for (G4int indexDepth = 0; indexDepth < depthNumber; indexDepth++) {
    depthPositionZ += depthSize[indexDepth] / 2.;
    if (indexDepth == 1) depthPositionZ += 0.001 * cm;
    moduleRinner = moduleRinner2;
    if (indexDepth == 0) moduleRinner = moduleRinner1;

    G4double absorberSize = absorberZ2;
    if (indexDepth < 3) absorberSize = absorberZ1;
    G4double absorberPositionZ =
      firstAbsorber[indexDepth] + gapSize + absorberSize / 2.0 - depthSize[indexDepth] / 2.0;

    solidDepths[indexDepth] =
      new G4Tubs(depthName, moduleRinner, moduleRouter, depthSize[indexDepth] / 2., modulePhistart,
                 moduleDeltaPhi);
    logicDepth[indexDepth] =
      new G4LogicalVolume(solidDepths[indexDepth],
                          lArMaterial,  // Caveat: in ATLAS Depth material is lAr
                          depthName);  // but I could put it to AirMaterial
    // logicDepth[indexDepth]->SetVisAttributes( DepthVisAttributes );//image dispaly
    logicDepth[indexDepth]->SetVisAttributes(G4VisAttributes::GetInvisible());

    physiDepth[indexDepth] =
      new G4PVPlacement(0, G4ThreeVector(0., 0., depthPositionZ), depthName, logicDepth[indexDepth],
                        physiModule, false, indexDepth, fCheckOverlaps);

    depthPositionZ += depthSize[indexDepth] / 2.0;
    if (indexDepth == 2) depthPositionZ += betweenWheel;  // end of depths positioning

    G4double slicePositionZ =
      firstAbsorber[indexDepth] + gapSize / 2.0 - depthSize[indexDepth] / 2.0;
    if (indexDepth > 0) sliceCopyNo += gapNumber[indexDepth - 1];
    if (indexDepth == 0) {
      sliceNo = 0;
      absorberNo = 0;
    }
    else {
      sliceNo = 1;
      absorberNo = 1;
    }
    if (indexDepth > 2) {
      sliceNo = 2;
      absorberNo = 2;
    }
    for (G4int indexSlice = 0; indexSlice < gapNumber[indexDepth]; indexSlice++) {
      /*physiSlice[sliceNo] =*/new G4PVPlacement(0,

                                                 G4ThreeVector(0., 0., slicePositionZ), sliceName,
                                                 logicSlice[sliceNo], physiDepth[indexDepth], false,
                                                 indexSlice + sliceCopyNo, fCheckOverlaps);
      slicePositionZ += absorberSize + gapSize;

      /*physiAbsorber[absorberNo] =*/new G4PVPlacement(
        0, G4ThreeVector(0, absorberPosY, absorberPositionZ), absorberName,
        logicAbsorber[absorberNo], physiDepth[indexDepth], false, indexSlice + sliceCopyNo + 1,
        fCheckOverlaps);

      absorberPositionZ += absorberSize + gapSize;
    }  // for indexSlice
  }  // for depthNumber

  // First absorber (placement)
  //
  G4double firstAbsorberPositionZ = firstAbsorber[0] / 2. - depthSize[0] / 2.0;
  /*physiFirstAbsorber =*/new G4PVPlacement(
    0, G4ThreeVector(0, absorberPosY, firstAbsorberPositionZ), firstAbsorbername,
    logicFirstAbsorber, physiDepth[0], false, 1, fCheckOverlaps);

  firstAbsorberPositionZ = firstAbsorber[3] / 2. - depthSize[3] / 2.0;
  /*physiAbsorber[1] =*/new G4PVPlacement(0, G4ThreeVector(0, absorberPosY, firstAbsorberPositionZ),
                                          absorberName, logicAbsorber[1], physiDepth[3], false, 1,
                                          fCheckOverlaps);

  // Tie rods in absorbers
  //
  G4String tieRodName = "ATLHECTBTieRod";
  G4String tieRodZoneName = "ATLHECTBTieRodZone";
  G4int indexRod;
  // auto TieRodVisAttributes = new G4VisAttributes();        //for image display
  // TieRodVisAttributes->SetForceSolid( true );              //for image display
  // TieRodVisAttributes->SetLineWidth(2.0);                  //for image display
  // TieRodVisAttributes->SetColour( G4Colour::Grey() );      //for image display

  solidAbsorberTieRod[0] =
    new G4Tubs(tieRodName, 0. * cm, tieRodDiameter[0] / 2., absorberZ1 / 2., 0. * deg, 360. * deg);
  solidAbsorberTieRod[1] =
    new G4Tubs(tieRodName, 0. * cm, tieRodDiameter[1] / 2., absorberZ2 / 2., 0. * deg, 360. * deg);
  logicAbsorberTieRod[0] =
    new G4LogicalVolume(solidAbsorberTieRod[0], FeMaterial, tieRodName, 0, 0, 0);
  logicAbsorberTieRod[1] =
    new G4LogicalVolume(solidAbsorberTieRod[1], FeMaterial, tieRodName, 0, 0, 0);

  // logicAbsorberTieRod[0]->SetVisAttributes(TieRodVisAttributes);   //for image display
  // logicAbsorberTieRod[1]->SetVisAttributes(TieRodVisAttributes);   //for image display
  logicAbsorberTieRod[0]->SetVisAttributes(G4VisAttributes::GetInvisible());
  logicAbsorberTieRod[1]->SetVisAttributes(G4VisAttributes::GetInvisible());

  for (G4int indexA = 0; indexA < 3; indexA++) {
    G4int indexR = 0;
    if (indexA > 1) indexR = 1;
    for (indexRod = 1; indexRod < 4; indexRod++) {
      /*physiAbsorberTieRod[indexR] =*/new G4PVPlacement(
        0, G4ThreeVector(tieRodPositionX[indexRod], -(tieRodPositionY[indexRod] + absorberPosY), 0),
        logicAbsorberTieRod[indexR], tieRodName, logicAbsorber[indexA], false, indexRod);

      /*physiAbsorberTieRod[indexR] =*/new G4PVPlacement(
        0,
        G4ThreeVector(-tieRodPositionX[indexRod], -(tieRodPositionY[indexRod] + absorberPosY), 0),
        logicAbsorberTieRod[indexR], tieRodName, logicAbsorber[indexA], false, indexRod);
    }

    /*physiAbsorberTieRod[indexR] =*/new G4PVPlacement(
      0, G4ThreeVector(tieRodPositionX[0], -(tieRodPositionY[0] + absorberPosY), 0),
      logicAbsorberTieRod[indexR], tieRodName, logicAbsorber[indexA], false, indexRod);
  }

  // Tie rods in slice
  //
  G4double ztie[2];
  ztie[0] = -0.227825 * cm;
  ztie[1] = 0.227825 * cm;
  G4double rodSize = 0.39435 * cm;
  //  G4double rodSize = 0.85*cm;
  for (G4int indexWheel = 0; indexWheel < 2; indexWheel++) {
    solidTieRod[indexWheel] = new G4Tubs(tieRodName, 0. * cm, spacerDiameter[indexWheel] / 2.,
                                         rodSize / 2., 0. * deg, 360. * deg);

    logicTieRod[indexWheel] =
      new G4LogicalVolume(solidTieRod[indexWheel], FeMaterial, tieRodName, 0, 0, 0);

    // logicTieRod[indexWheel]->SetVisAttributes(TieRodVisAttributes);//for image
    //  display
    logicTieRod[indexWheel]->SetVisAttributes(G4VisAttributes::GetInvisible());

    if (TieRodZone) {  // true: HECversion == "standard_np_zone"
      solidTieRodZone[indexWheel] =
        new G4Tubs(tieRodZoneName, spacerDiameter[indexWheel] / 2.,
                   (spacerDiameter[indexWheel] / 2.) + 2. * mm, rodSize / 2., 0. * deg, 360. * deg);
      logicTieRodZone[indexWheel] =
        new G4LogicalVolume(solidTieRodZone[indexWheel], lArMaterial, tieRodZoneName, 0, 0, 0);
      // logicTieRodZone[indexWheel]->SetVisAttributes(SliceVisAttributes);//for image display
      logicTieRodZone[indexWheel]->SetVisAttributes(G4VisAttributes::GetInvisible());
    }
  }
  for (G4int numberSlice = 0; numberSlice < 3; numberSlice++) {
    G4int numberTie = 0;
    if (numberSlice == 2) numberTie = 1;
    for (indexRod = 1; indexRod < 4; indexRod++) {
      for (G4int iz = 0; iz < 2; iz++) {
        /*physiTieRod[numberTie] =*/new G4PVPlacement(
          0, G4ThreeVector(tieRodPositionX[indexRod], -tieRodPositionY[indexRod], ztie[iz]),
          logicTieRod[numberTie], tieRodName, logicSlice[numberSlice], false, indexRod);

        /*physiTieRod[numberTie] =*/new G4PVPlacement(
          0, G4ThreeVector(-tieRodPositionX[indexRod], -tieRodPositionY[indexRod], ztie[iz]),
          logicTieRod[numberTie], tieRodName, logicSlice[numberSlice], false, indexRod);

        if (TieRodZone) {  // true: HECversion == "standard_np_zone"
          /*physiTieRodZone[numberTie] =*/new G4PVPlacement(
            0, G4ThreeVector(tieRodPositionX[indexRod], -tieRodPositionY[indexRod], ztie[iz]),
            logicTieRodZone[numberTie], tieRodZoneName, logicSlice[numberSlice], false, indexRod);

          /*physiTieRodZone[numberTie] =*/new G4PVPlacement(
            0, G4ThreeVector(-tieRodPositionX[indexRod], -tieRodPositionY[indexRod], ztie[iz]),
            logicTieRodZone[numberTie], tieRodZoneName, logicSlice[numberSlice], false, indexRod);
        }
      }  // for iz
    }  // for indexRod

    for (G4int iz1 = 0; iz1 < 2; iz1++) {
      /*physiTieRod[numberTie] =*/new G4PVPlacement(
        0, G4ThreeVector(tieRodPositionX[0], -tieRodPositionY[0], ztie[iz1]),
        logicTieRod[numberTie], tieRodName, logicSlice[numberSlice], false, indexRod - 3);

      if (TieRodZone) {  // HECversion == "standard_np_zone"
        /*physiTieRodZone[numberTie] =*/new G4PVPlacement(
          0, G4ThreeVector(tieRodPositionX[0], -tieRodPositionY[0], ztie[iz1]),
          logicTieRodZone[numberTie], tieRodZoneName, logicSlice[numberSlice], false, indexRod - 3);
      }
    }  // for iz1
  }  // for numberSlice

  // Electronic boards of a slice
  //
  // auto ElectrodeVisAttributes = new G4VisAttributes();        //for image dislay
  // ElectrodeVisAttributes->SetForceWireframe( true );          //for image display
  // ElectrodeVisAttributes->SetLineWidth(2.0);                  //for image display
  // ElectrodeVisAttributes->SetColour( G4Colour::Yellow() );    //for image display

  // auto CopperVisAttributes = new G4VisAttributes();           //for image display
  // CopperVisAttributes->SetForceSolid( true );                 //for image display
  // CopperVisAttributes->SetLineWidth(2.0);                     //for image display
  // CopperVisAttributes->SetColour( G4Colour::Red() );          //for image display

  G4String electrodeName = "ATLHECTB::Electrode";
  G4String copperName = "ATLHECTB::Copper";
  for (G4int indexBoard = 0; indexBoard < 3; indexBoard++) {
    if (indexBoard == 0)
      moduleRinner = moduleRinner1;
    else
      moduleRinner = moduleRinner2;
    // for(G4int indexKapton=0; indexKapton<3; indexKapton++) {
    G4int indexKapton = 1;
    solidPadBoard = new G4Tubs(copperName, moduleRinner, moduleRouter, copperPad / 2.,
                               modulePhistart, moduleDeltaPhi);

    logicPadBoard = new G4LogicalVolume(solidPadBoard, CuMaterial, copperName);
    // logicPadBoard->SetVisAttributes(CopperVisAttributes); //for image display
    logicPadBoard->SetVisAttributes(G4VisAttributes::GetInvisible());

    solidEstBoard =
      new G4Tubs(electrodeName,
                 // moduleRinner,moduleRouter,(kaptonWidth[indexKapton])/2.,
                 moduleRinner, moduleRouter, (kaptonWidth[indexKapton] / 2. + kaptonWidth[0]),
                 modulePhistart, moduleDeltaPhi);

    logicEstBoard = new G4LogicalVolume(solidEstBoard, KaptonMaterial, electrodeName);
    // logicEstBoard->SetVisAttributes(ElectrodeVisAttributes);//for image display
    logicEstBoard->SetVisAttributes(G4VisAttributes::GetInvisible());

    G4double kaptonPositionZ = kaptonPosition[indexKapton] - gapSize / 2.;
    physiEstBoard = new G4PVPlacement(0, G4ThreeVector(0, 0, kaptonPositionZ), logicEstBoard,
                                      electrodeName, logicSlice[indexBoard], false, indexKapton);

    if (indexKapton == 1) {
      /*physiPadBoard =*/new G4PVPlacement(0, G4ThreeVector(0, 0, 0), copperName, logicPadBoard,
                                           physiEstBoard, false, indexKapton);
    }  // if
    //}//for indexKapton
  }  // for indexBoard

  // As reported by Andrey Kiryunin on December 2022,
  // it is better to include two scintillator from the beam line
  // in the simulation.
  //
  // C9H10 as described in atlas original code:
  // HECTB/LArCalorimeter/LArG4TB/LArG4TBEmecHecLArG4TBEmecHec/LArTBEmecHecFrontBeamConstruction.cc
  //
  G4int ncomponents_scint;
  G4double z_scint, a_scint;
  G4String name_scint, symbol_scint;
  G4Material* Scintillator = new G4Material("Scintillator", 1.032 * g / cm3, ncomponents_scint = 2);
  G4Element* elH_scint = new G4Element(name_scint = "Hydrogen", symbol_scint = "H", z_scint = 1.,
                                       a_scint = 1.01 * g / mole);
  G4Element* elC_scint = new G4Element(name_scint = "Carbon", symbol_scint = "C", z_scint = 6.,
                                       a_scint = 12.01 * g / mole);
  Scintillator->AddElement(elC_scint, natoms = 9);
  Scintillator->AddElement(elH_scint, natoms = 10);

  // Create scintilators and position them
  //
  G4double bfas_rmin = 0.0 * cm;  // from atlas original code
  G4double bfas_rmax = 5.0 * cm;  // from atlas original code
  G4double bfas_dz = 0.375 * cm;  // from atlas original code
  G4Tubs* shapeScint =
    new G4Tubs("LArG4TB::FrontBeam::Scint", bfas_rmin, bfas_rmax, bfas_dz, 0. * deg, 360. * deg);
  G4LogicalVolume* logScint =
    new G4LogicalVolume(shapeScint, Scintillator, "LArG4TB::FrontBeam::Scint", 0, 0, 0);
  const G4double Scint_X = -9.0 * cm;
  const G4double Scint_Y = 172.0 * cm;
  const G4double Scint_Z[2] = {207.5 * cm, 220.0 * cm};
  for (std::size_t i = 0; i < 2; i++) {
    // original atlas code has 3 scintillators but Andrey suggested to use only 2 here
    /*G4VPhysicalVolume *bfsa_phys =*/new G4PVPlacement(
      0, G4ThreeVector(Scint_X, Scint_Y, Scint_Z[i]), logScint, "LArG4TB::FrontBeam::Scint",
      logicWorld, false, i + 2);
  }

  // Scintillator vis attributes
  //
  auto ScintillatorVisAttr = new G4VisAttributes();
  ScintillatorVisAttr->SetVisibility(true);
  ScintillatorVisAttr->SetForceSolid(true);
  ScintillatorVisAttr->SetColor(G4Color::Yellow());
  logScint->SetVisAttributes(ScintillatorVisAttr);

  // Return physical world
  //
  G4cout << "----------> ATLHECTB geometry built <----------" << G4endl;
  return physiWorld;
}

//**************************************************
