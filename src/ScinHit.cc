#include "ScinHit.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include "globals.hh"

namespace MDT
{

G4ThreadLocal G4Allocator<ScinHit>* ScinHitAllocator;

ScinHit::ScinHit()
{

}

ScinHit::~ScinHit()
{

}

G4bool ScinHit::operator==(const ScinHit &/*right*/) const
{
  return false;
}

void ScinHit::Draw()
{
  auto visManager = G4VVisManager::GetConcreteInstance();
  if (! visManager) return;

  G4Transform3D trans(fRot.inverse(),fPos);
  G4VisAttributes attribs;
  auto pVA = fLogV->GetVisAttributes();
  if (pVA) attribs = *pVA;
  G4Colour colour(0.,1.,1.);
  attribs.SetColour(colour);
  attribs.SetForceSolid(true);
  visManager->Draw(*fLogV,attribs,trans);
}

const std::map<G4String,G4AttDef>* ScinHit::GetAttDefs() const
{
  G4bool isNew;
  auto store = G4AttDefStore::GetInstance("ScinHit",isNew);

  if (isNew) {
    (*store)["HitType"]
      = G4AttDef("HitType","Hit Type","Physics","","G4String");

    (*store)["ID"]
      = G4AttDef("ID","ID","Physics","","G4int");

    (*store)["Time"]
      = G4AttDef("Time","Time","Physics","G4BestUnit","G4double");

    (*store)["EDep"]
    = G4AttDef("EDep","energy deposit","Physics","G4BestUnit","G4double");

    (*store)["Pos"]
      = G4AttDef("Pos","Position","Physics","G4BestUnit","G4ThreeVector");

    (*store)["LVol"]
      = G4AttDef("LVol","Logical Volume","Physics","","G4String");
  }
  return store;
}

std::vector<G4AttValue>* ScinHit::CreateAttValues() const
{
  auto values = new std::vector<G4AttValue>;

  values
    ->push_back(G4AttValue("HitType","ScinHit",""));
  values
    ->push_back(G4AttValue("ID",G4UIcommand::ConvertToString(fID),""));
  values
    ->push_back(G4AttValue("Time",G4BestUnit(fTime,"Time"),""));
  values
    ->push_back(G4AttValue("EDep",G4BestUnit(eDep,"EDep"),""));
  values
    ->push_back(G4AttValue("Pos",G4BestUnit(fPos,"Length"),""));

  if (fLogV)
    values->push_back(G4AttValue("LVol",fLogV->GetName(),""));
  else
    values->push_back(G4AttValue("LVol"," ",""));

  return values;
}


void ScinHit::Print()
{
  G4cout << "  Tube[" << fID << "] " << fTime/ns << " (nsec)" << G4endl;
}

}
