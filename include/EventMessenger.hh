#ifndef MDTEventMessenger_h
#define MDTEventMessenger_h

#include "globals.hh"
#include "G4UImessenger.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

namespace MDT
{

class EventAction;

class EventMessenger: public G4UImessenger
{

public:
    EventMessenger(EventAction* );
    ~EventMessenger() override;

    void SetNewValue(G4UIcommand*, G4String) override;

private:
    EventAction* fEventAction = nullptr;

    G4UIdirectory* fDirectory = nullptr;
    G4UIdirectory* fEventDirectory = nullptr;
    G4UIcmdWithADoubleAndUnit* fScinThresholdCmd = nullptr;
    G4UIcmdWithADoubleAndUnit* fTubeThresholdCmd = nullptr;
};

}

#endif
