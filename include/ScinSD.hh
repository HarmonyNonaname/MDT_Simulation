#ifndef MDTScinSD_h
#define MDTScinSD_h

#include "G4VSensitiveDetector.hh"
#include "ScinHit.hh"

#include "globals.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

namespace MDT
{

class ScinSD : public G4VSensitiveDetector
{

public:
    ScinSD(G4String SDname, G4String HCname);
    ~ScinSD() override;

public:
    G4bool ProcessHits(G4Step *step, G4TouchableHistory *ROhist) override;
    void Initialize(G4HCofThisEvent *HCE) override;
    void EndOfEvent(G4HCofThisEvent *HCE);

private:
    ScinHitsCollection* fHitsCollection = nullptr;
    G4int fHCID = -1;

};

}

#endif