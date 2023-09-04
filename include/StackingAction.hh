#ifndef MDTStackingAction_h
#define MDTStackingAction_h

#include "G4UserStackingAction.hh"

#include "globals.hh"

namespace MDT
{

class StackingAction : public G4UserStackingAction
{
  public:
    StackingAction();
    ~StackingAction() override;

    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*) override;
};

}

#endif

