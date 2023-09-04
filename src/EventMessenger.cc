#include "EventMessenger.hh"
#include "EventAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

namespace MDT
{

EventMessenger::EventMessenger(EventAction* event) : fEventAction(event)
{
    fDirectory = new G4UIdirectory("/MDT/");
    fDirectory->SetGuidance("UI commands specific to this MDT experiment.");

    fEventDirectory = new G4UIdirectory("/MDT/event/");
    fEventDirectory->SetGuidance("Event Action control");

    fScinThresholdCmd = new G4UIcmdWithADoubleAndUnit("/MDT/event/setScinThreshold",this);
    fScinThresholdCmd->SetGuidance("Set Scintillators' energy threshold.");
    fScinThresholdCmd->SetParameterName("threshold",false);
    fScinThresholdCmd->SetUnitCategory("Energy");
    fScinThresholdCmd->AvailableForStates(G4State_Idle);

    fTubeThresholdCmd = new G4UIcmdWithADoubleAndUnit("/MDT/event/setTubeThreshold",this);
    fTubeThresholdCmd->SetGuidance("Set Tubes' energy threshold.");
    fTubeThresholdCmd->SetParameterName("threshold",false);
    fTubeThresholdCmd->SetUnitCategory("Energy");
    fTubeThresholdCmd->AvailableForStates(G4State_Idle);

}

EventMessenger::~EventMessenger()
{
    delete fScinThresholdCmd;
    delete fDirectory;
    delete fEventDirectory;
}

void EventMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if( command == fScinThresholdCmd ) { fEventAction->SetScinThreshold(fScinThresholdCmd->GetNewDoubleValue(newValue));}
    if( command == fTubeThresholdCmd ) { fEventAction->SetTubeThreshold(fTubeThresholdCmd->GetNewDoubleValue(newValue));}
}

}
