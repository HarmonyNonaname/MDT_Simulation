#include "StackingAction.hh"

#include "G4Track.hh"
#include "G4NeutrinoE.hh"

#include "globals.hh"

namespace MDT
{

StackingAction::StackingAction()
{

}

StackingAction::~StackingAction()
{

}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* track)
{
  //keep primary particle
  if (track->GetParentID() == 0) return fUrgent;

  //kill secondary particle
  if (track->GetParentID() != 0) return fKill;
  else return fUrgent;
}

}

