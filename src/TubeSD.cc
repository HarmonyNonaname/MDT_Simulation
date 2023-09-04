#include "TubeSD.hh"
#include "TubeHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "globals.hh"

namespace MDT
{

TubeSD::TubeSD(G4String SDname, G4String HCname)
: G4VSensitiveDetector(SDname)
{
    G4cout<<"Creating SD with name: "<<SDname<<G4endl;
    collectionName.insert(HCname);
}

TubeSD::~TubeSD()
{

}

G4bool TubeSD::ProcessHits(G4Step *step, G4TouchableHistory *ROhist)
{
    auto edep = step->GetTotalEnergyDeposit();

    G4Track* track = step->GetTrack();
    G4int trackID = track->GetTrackID();
    if (edep==0.) return false;

    auto preStepPoint = step->GetPreStepPoint();
    auto touchable = preStepPoint->GetTouchable();
    auto copyNo = touchable->GetVolume()->GetCopyNo();
    auto hitTime = preStepPoint->GetGlobalTime();

    TubeHit* hit = new TubeHit();
    auto physical = touchable->GetVolume();
    hit->SetTime(hitTime);
    hit->SeteDep(edep);
    hit->SetLogV(physical->GetLogicalVolume());
    auto transform = touchable->GetHistory()->GetTopTransform();
    transform.Invert();
    hit->SetRot(transform.NetRotation());
    hit->SetPos(track->GetPosition());
    fHitsCollection->insert(hit);

    return true;
}

void TubeSD::Initialize(G4HCofThisEvent *HCE)
{
    fHitsCollection = new TubeHitsCollection(SensitiveDetectorName, collectionName[0]);

    if(fHCID<0) fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    HCE->AddHitsCollection(fHCID, fHitsCollection);
}

void TubeSD::EndOfEvent(G4HCofThisEvent *HCE)
{

}

}