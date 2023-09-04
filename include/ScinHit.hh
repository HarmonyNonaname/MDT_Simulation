#ifndef MDTScinHit_h
#define MDTScinHit_h

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

#include "globals.hh"

class G4AttDef;
class G4AttValue;

namespace MDT
{

class ScinHit : public G4VHit
{

public:
    ScinHit();
    ~ScinHit();

    G4double GetEdep() const { return eDep; }
    G4int GetNumber() const { return TubeNumber; }
    
    ScinHit& operator=(const ScinHit &right) = default;
    G4bool operator==(const ScinHit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void* aHit);

    void Draw() override;
    const std::map<G4String,G4AttDef>* GetAttDefs() const override;
    std::vector<G4AttValue>* CreateAttValues() const override;

    void Print() override;

    void SetID(G4int fid) { fID = fid; }
    G4int GetID() const { return fID; }

    void SetTime(G4double val) { fTime = val; }
    G4double GetTime() const { return fTime; }

    void SeteDep(G4double val) { eDep = val; }
    G4double GeteDep() const { return eDep; }

    void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    G4ThreeVector GetPos() const { return fPos; }

    void SetRot(G4RotationMatrix rmat) { fRot = rmat; }
    G4RotationMatrix GetRot() const { return fRot; }

    void SetLogV(G4LogicalVolume* val) { fLogV = val; }
    const G4LogicalVolume* GetLogV() const { return fLogV; }

private:
    G4int fID = -1;
    G4int TubeNumber;
    G4double fTime = 0.;
    G4double eDep = 0.;
    G4ThreeVector fPos;
    G4RotationMatrix fRot;
    const G4LogicalVolume* fLogV = nullptr;
};

typedef G4THitsCollection<ScinHit> ScinHitsCollection;

extern G4ThreadLocal G4Allocator<ScinHit>* ScinHitAllocator;

inline void* ScinHit::operator new(size_t)
{
  if (!ScinHitAllocator) {
       ScinHitAllocator = new G4Allocator<ScinHit>;
  }
  return (void*)ScinHitAllocator->MallocSingle();
}

inline void ScinHit::operator delete(void*aHit)
{
  ScinHitAllocator->FreeSingle((ScinHit*) aHit);
}

}

#endif