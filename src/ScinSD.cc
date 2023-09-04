#include "ScinSD.hh"
#include "ScinHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "globals.hh"

namespace MDT
{

ScinSD::ScinSD(G4String SDname, G4String HCname)
: G4VSensitiveDetector(SDname)
{
    G4cout<<"Creating SD with name: "<<SDname<<G4endl;
    collectionName.insert(HCname);
}

ScinSD::~ScinSD()
{

}

G4bool ScinSD::ProcessHits(G4Step *step, G4TouchableHistory *ROhist)
{
    auto edep = step->GetTotalEnergyDeposit();

    G4Track* track = step->GetTrack();
    G4int trackID = track->GetTrackID();
    if (edep==0.) return false;

    auto preStepPoint = step->GetPreStepPoint();
    auto touchable = preStepPoint->GetTouchable();
    auto copyNo = touchable->GetVolume()->GetCopyNo();
    auto hitTime = preStepPoint->GetGlobalTime();

    ScinHit* hit = new ScinHit();
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

void ScinSD::Initialize(G4HCofThisEvent *HCE)
{
    fHitsCollection = new ScinHitsCollection(SensitiveDetectorName, collectionName[0]);

    if(fHCID<0) fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    HCE->AddHitsCollection(fHCID, fHitsCollection);
}

void ScinSD::EndOfEvent(G4HCofThisEvent *HCE)
{

}

}