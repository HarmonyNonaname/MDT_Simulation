#include "DetectorConstruction.hh"
#include "TubeSD.hh"
#include "ScinSD.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"

#include "globals.hh"

namespace MDT
{

DetectorConstruction::DetectorConstruction()
{

}

DetectorConstruction::~DetectorConstruction()
{

}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Envelope parameters//
  G4double env_sizeX = 160.*cm, env_sizeY = 160.*cm, env_sizeZ = 160.*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4bool checkOverlaps = true;

  
  // World //
  G4double world_sizeX = 1.0*env_sizeX;
  G4double world_sizeY = 1.0*env_sizeY;
  G4double world_sizeZ  = 1.0*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box* solidWorld = new G4Box("World", world_sizeX, world_sizeY, world_sizeZ);

  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");

  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps); 

  // Envelope //
  G4Box* solidEnv = new G4Box("Envelope", env_sizeX, env_sizeY, env_sizeZ);

  G4LogicalVolume* logicEnv = new G4LogicalVolume(solidEnv, env_mat, "Envelope");

  new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);

  // Scintillator Material //
  G4Material* Scin_mat = nist->FindOrBuildMaterial("G4_BGO");

  // Tube Material---> //
  G4int natoms, ncomponents;
  G4String name, symbol;
  G4double a,z;
  G4double density, pressure, temperature, fractionmass;

  // Element Ar //
  a = 39.96*g/mole;
  G4Element* elAr = new G4Element(name="elAr", symbol="Ar", z=18., a);

  // Material CO2 //
  a = 12.01*g/mole;
  G4Element* elC = new G4Element(name="elC", symbol="C", z=6., a);
  a = 16.00*g/mole;
  G4Element* elO = new G4Element(name="elO", symbol="O", z=8., a);

  density = 1.98*mg/cm3;
  G4Material* CO2 = new G4Material(name="CO2", density, ncomponents=2);
  CO2->AddElement(elC, natoms=1);
  CO2->AddElement(elO, natoms=2);

  // Material Ar-CO2 93-7 //
  density = 7.10*g/cm3;
  pressure = 3.*atmosphere;
  temperature = 350.*kelvin;
  G4Material* ArCO2 = new G4Material(name="ArCO2", density, ncomponents=2, kStateGas, temperature, pressure);
  ArCO2->AddElement(elAr, fractionmass=93.*perCent);
  ArCO2->AddMaterial(CO2, fractionmass=7.*perCent);
  // <---Tube Material //

  // ---> Scintillator Shape //
  G4double Scin1_length = 47.9*cm, Scin1_width = 22.5*cm, Scin1_height = 1.75*cm; //temporary Size
  G4double Scin2_length = 39.5*cm, Scin2_width = 10.05*cm, Scin2_height = 1.5*cm;
  G4double Scin3_length = 34.65*cm, Scin3_width = 6.75*cm, Scin3_height = 1.5*cm;

  G4Box* Scin1_Shape = new G4Box("Scin1_Shape", Scin1_width, Scin1_length, Scin1_height);
  G4Box* Scin2_Shape = new G4Box("Scin2_Shape", Scin2_width, Scin2_length, Scin2_height);
  G4Box* Scin3_Shape = new G4Box("Scin3_Shape", Scin3_width, Scin3_length, Scin3_height);
  // <--- Scintilattor Shape //


  // ---> Tube Shape //
  G4double tubeRmin = 25*um, tubeRmax = 1.5*cm, tubeZ = 42.*cm, pSPhi = 0.*rad, pDPhi = 2.*CLHEP::pi*rad; // Shape of a single tube
  G4Tubs* TubeShape = new G4Tubs("TubeShape", tubeRmin, tubeRmax, tubeZ, pSPhi, pDPhi);
  // <--- Tube Shape //

  // ---> Scintillators Placement //

  G4double tube_distance = 22.5*cm; // distance between layers
  G4double Scin1_pos_x=0*cm, Scin1_pos_y=0*cm, Scin1_pos_z=2.*tube_distance+3*3*std::sqrt(3)*tubeRmax+41.75*cm; // Scintillator1 position
  G4double Scin2_pos_x=10.05*cm, Scin2_pos_y=0*cm, Scin2_pos_z=-31.*cm; // Scintillator2 position
  G4double Scin3_pos_x=-6.75*cm, Scin3_pos_y=0*cm, Scin3_pos_z=-31.*cm; // Scintillator2 position
  G4ThreeVector Scin1_pos = G4ThreeVector(Scin1_pos_x, Scin1_pos_y, Scin1_pos_z);
  G4ThreeVector Scin2_pos = G4ThreeVector(Scin2_pos_x, Scin2_pos_y, Scin2_pos_z);
  G4ThreeVector Scin3_pos = G4ThreeVector(Scin3_pos_x, Scin3_pos_y, Scin3_pos_z);
  G4RotationMatrix* Scin1_rot = new G4RotationMatrix();
  G4RotationMatrix* Scin2_rot = new G4RotationMatrix();
  G4RotationMatrix* Scin3_rot = new G4RotationMatrix();
  Scin1_rot->rotateX(0.*deg);
  Scin2_rot->rotateX(0.*deg);
  Scin3_rot->rotateX(0.*deg);

  Scin1_Logic = new G4LogicalVolume(Scin1_Shape, Scin_mat, "Scin1_Logic");
  G4Colour trans_blue(0., 0., 1., 0.5);
  G4VisAttributes* Scin1_Attributes = new G4VisAttributes(trans_blue);
  Scin1_Logic->SetVisAttributes(Scin1_Attributes);

  Scin2_Logic = new G4LogicalVolume(Scin2_Shape, Scin_mat, "Scin2_Logic");
  G4VisAttributes* Scin2_Attributes = new G4VisAttributes(trans_blue);
  Scin2_Logic->SetVisAttributes(Scin2_Attributes);

  Scin3_Logic = new G4LogicalVolume(Scin3_Shape, Scin_mat, "Scin3_Logic");
  G4VisAttributes* Scin3_Attributes = new G4VisAttributes(trans_blue);
  Scin3_Logic->SetVisAttributes(Scin3_Attributes);

  G4VPhysicalVolume* Scin1_Phys = new G4PVPlacement(Scin1_rot, Scin1_pos, Scin1_Logic, "Scin1", logicEnv, false, 0, checkOverlaps);
  G4VPhysicalVolume* Scin2_Phys = new G4PVPlacement(Scin2_rot, Scin2_pos, Scin2_Logic, "Scin2", logicEnv, false, 0, checkOverlaps);
  G4VPhysicalVolume* Scin3_Phys = new G4PVPlacement(Scin3_rot, Scin3_pos, Scin3_Logic, "Scin3", logicEnv, false, 0, checkOverlaps);

  // <--- Scintillators Placement //

  // ---> Tube Placement //

  G4double tube_pos_x=0.*cm, tube_pos_y=0.*cm, tube_pos_z=0.*cm; // tube_position
  G4double x_total = 48.*cm, y_total = 2.*tube_distance+3*3*std::sqrt(3)*tubeRmax, z_total = 0.*cm;
  G4ThreeVector tube_pos = G4ThreeVector(tube_pos_x, tube_pos_y, tube_pos_z);
  G4RotationMatrix* tube_rot = new G4RotationMatrix();

  tube_rot->rotateX(90.*deg);

  //std::vector<G4LogicalVolume* > TubeRow(num_row, NULL);
  //std::vector<std::vector<G4LogicalVolume* >> TubeCol(num_col, TubeRow);
  //std::vector<std::vector<std::vector<G4LogicalVolume* >>> TubeLogic(num_group, TubeCol);

  G4String TubeLogName="";
  for(int i = 0; i < 6; i++){ // group
    for(int j = 0; j < 3; j++){ // col
      for(int k = 0; k < 8; k++){ // row
        TubeLogName = "TubeLogic_"+std::to_string(i)+"_"+std::to_string(j)+"_"+std::to_string(k);
        TubeLogic[i][j][k] = new G4LogicalVolume(TubeShape, ArCO2, TubeLogName);
      }
    }
  }

  G4Colour trans_red(1., 0., 0., 0.5);
  G4VisAttributes* TubeAttributes = new G4VisAttributes(trans_red);

  for(int i = 0; i < 6; i++){
    for(int j = 0; j < 3; j++){
      for(int k = 0; k < 8; k++){
        TubeLogic[i][j][k]->SetVisAttributes(TubeAttributes);
      }
    }
  }

  G4String TubePhyName="";
  G4VPhysicalVolume *Tube_Phys[6][3][8] = {nullptr};

  for(int i = 0; i < 6; i++){
    for(int j = 0; j < 3; j++){
      for(int k = 0; k < 8; k++){
        TubePhyName = "TubePhy_"+std::to_string(i)+"_"+std::to_string(j)+"_"+std::to_string(k);
        //G4LogicalVolume* Tube_Logic_Temp = new G4LogicalVolume(TubeShape, ArCO2, TubePhyName);
        //TubeLogic[i][j][k] = Tube_Logic_Temp;

        tube_pos_x = i%2*24.0*cm + (j+1)%2*tubeRmax + k*2.*tubeRmax;
        tube_pos_y = 0.*cm + 0.5*tubeZ;
        tube_pos_z = (2-i/2)*tube_distance - (j-2)*std::sqrt(3)*tubeRmax - tubeZ;

        tube_pos_x -= 0.5*x_total;
        tube_pos_y -= 0.25*y_total;
        //tube_pos_z += z_total;
        
        //tube_pos_x += 25.5*cm;
        tube_pos_z += 7.75*cm + 0.5*y_total + tubeRmax;

        tube_pos = G4ThreeVector(tube_pos_x, tube_pos_y, tube_pos_z);
        Tube_Phys[i][j][k] = new G4PVPlacement(tube_rot, tube_pos, TubeLogic[i][j][k], TubePhyName, logicEnv, false, 0, checkOverlaps);
      }
    }
  }

  // <--- Tube Placement //

  // ---> Air Energy Cut //
  G4double air_maxStep = 0.5*cm;
  //G4double air_maxTrack = 1000.*cm;
  //G4double air_maxTime = 1000.*s;
  //G4double air_minEkin = 250.*eV;
  //air_limit = new G4UserLimits(air_maxStep);
  air_limit = new G4UserLimits(air_maxStep);
  logicWorld->SetUserLimits(air_limit);
  logicEnv->SetUserLimits(air_limit);
  Scin1_Logic->SetUserLimits(air_limit);
  Scin2_Logic->SetUserLimits(air_limit);
  Scin3_Logic->SetUserLimits(air_limit);

  // <--- Air Energy Cut //

  // ---> Tube Energy Cut //
  
  G4double tube_maxStep = 1.*cm;
  G4double tube_maxTrack = 50.*cm;
  G4double tube_maxTime = 1000.*s;
  G4double tube_minEkin = 250.*eV;
  tube_limit = new G4UserLimits(tube_maxStep);
  for(int i = 0; i < 6; i++){
    for(int j = 0; j < 3; j++){
      for(int k = 0; k < 8; k++){
        TubeLogic[i][j][k]->SetUserLimits(tube_limit);
      }
    }
  }
  
  // <---Tube Energyb Cut //

  // always return the physical World
  return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
  auto SDManager = G4SDManager::GetSDMpointer();
  G4String SDname="";
  G4String HCname="";

  TubeSD* Tube[6][3][8] = {nullptr};

  for(int i = 0; i < 6; i++){
    for(int j = 0; j < 3; j++){
      for(int k = 0; k < 8; k++){
        SDname = "SDTube_"+std::to_string(i)+"_"+std::to_string(j)+"_"+std::to_string(k);
        HCname = "HCTube_"+std::to_string(i)+"_"+std::to_string(j)+"_"+std::to_string(k);
        Tube[i][j][k] = new TubeSD(SDname, HCname);
        SDManager->AddNewDetector(Tube[i][j][k]);
        TubeLogic[i][j][k]->SetSensitiveDetector(Tube[i][j][k]);
      }
    }
  }


  ScinSD* Scin1 = new ScinSD("SDScin1", "HCScin1");
  ScinSD* Scin2 = new ScinSD("SDScin2", "HCScin2");
  ScinSD* Scin3 = new ScinSD("SDScin3", "HCScin3");
  SDManager->AddNewDetector(Scin1);
  SDManager->AddNewDetector(Scin2);
  SDManager->AddNewDetector(Scin3);
  Scin1_Logic->SetSensitiveDetector(Scin1);
  Scin2_Logic->SetSensitiveDetector(Scin2);
  Scin3_Logic->SetSensitiveDetector(Scin3);

}

}
