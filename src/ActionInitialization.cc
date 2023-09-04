#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"

namespace MDT
{

ActionInitialization::ActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}


void ActionInitialization::BuildForMaster() const
{
  RunAction* runAction = new RunAction;
  SetUserAction(runAction);
}

void ActionInitialization::Build() const
{

  SetUserAction(new PrimaryGeneratorAction("setup.file"));
  RunAction *runAction = new RunAction;
  SetUserAction(runAction);
  EventAction *eventAction = new EventAction(runAction);
  SetUserAction(eventAction);
  SetUserAction(new StackingAction);
  SetUserAction(new SteppingAction(eventAction));
}

}
