#ifndef MDTPrimaryGeneratorAction_h
#define MDTPrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include "Randomize.hh"
#include "RNGWrapper.hh"

class G4GeneralParticleSource ;
class G4Event;
class G4Box;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(const char * filename);
    ~PrimaryGeneratorAction() override;

    void GeneratePrimaries(G4Event* anEvent);

  private:
    G4GeneralParticleSource* particleGun;
};



#endif
