#ifndef MDTDetectorConstruction_h
#define MDTDetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4UserLimits.hh"
#include "G4Tubs.hh"

#include "globals.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;
class G4VSensitiveDetector;
class G4VisAttributes;
class G4GenericMessenger;

namespace MDT
{

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  ~DetectorConstruction() override;

  G4VPhysicalVolume* Construct() override;
  void ConstructSDandField() override;

private:
  G4LogicalVolume* fScoringVolume = nullptr;
  G4UserLimits* tube_limit = nullptr;
  G4UserLimits* air_limit = nullptr;

  G4LogicalVolume* TubeLogic[6][3][8] = {nullptr};
  G4LogicalVolume* Scin1_Logic = {nullptr};
  G4LogicalVolume* Scin2_Logic = {nullptr};
  G4LogicalVolume* Scin3_Logic = {nullptr};

};

}

#endif
