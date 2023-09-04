#ifndef MDTEventAction_h
#define MDTEventAction_h

#include "G4UserEventAction.hh"
#include "EventMessenger.hh"
#include "globals.hh"

namespace MDT
{

class RunAction;

class EventAction : public G4UserEventAction
{

public:
    EventAction(RunAction* runAction);
    ~EventAction() override;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;
  
    //void AddEdep(G4double edep, G4int index);

    void SetScinThreshold(G4double energy);
    void SetTubeThreshold(G4double energy);
    G4double GetScinThreshold() { return ScinThreshold; }
    G4double GetTubeThreshold() { return TubeThreshold; }

private:
    RunAction* fRunAction = nullptr;

    EventMessenger* fMessenger = nullptr;
    G4int fCollID_Scin1 = -1;
    G4int fCollID_Scin2 = -1;
    G4int fCollID_Scin3 = -1;
    G4int fCollID_Tube[6][3][8] = {0};

    G4double ScinThreshold = 0.;
    G4double TubeThreshold = 0.;

    // G4Accumulable<G4double> fEdep1 = 0.;
    //G4Accumulable<G4double> fEdep2 = 0.;
};

}

#endif


