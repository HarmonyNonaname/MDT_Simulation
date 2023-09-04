#ifndef MDTActionInitialization_h
#define MDTActionInitialization_h

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

namespace MDT
{

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization();
    ~ActionInitialization() override;

    void BuildForMaster() const override;
    void Build() const override;
};

}

#endif
