#ifndef MDTTubeSD_h
#define MDTTubeSD_h

#include "G4VSensitiveDetector.hh"
#include "TubeHit.hh"

#include "globals.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

namespace MDT
{

class TubeSD : public G4VSensitiveDetector
{

public:
    TubeSD(G4String SDname, G4String HCname);
    ~TubeSD() override;

public:
    G4bool ProcessHits(G4Step *step, G4TouchableHistory *ROhist) override;
    void Initialize(G4HCofThisEvent *HCE) override;
    void EndOfEvent(G4HCofThisEvent *HCE);

private:
    TubeHitsCollection* fHitsCollection = nullptr;
    G4int fHCID = -1;

};

}

#endif