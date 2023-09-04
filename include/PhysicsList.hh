#ifndef MDTPhysicsList_h
#define MDTPhysicsList_h

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

namespace MDT
{


class PhysicsList: public G4VUserPhysicsList
{
  public:
    PhysicsList();
    ~PhysicsList();

  protected:
    // Construct particle and physics
    void ConstructParticle();
    void ConstructProcess();

    // Define tracking cuts (step length, etc)
    void SetCuts();
   
  protected:
    // these methods Construct particles 
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();
    void ConstructIons();

  protected:
    // these methods Construct physics processes and register them
    void ConstructGeneral();
    void ConstructInteractions();
};

}
#endif
