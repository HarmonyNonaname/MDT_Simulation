/// \file MDT.cc
/// \brief Main program of the MDT

#include <fstream>
#include <math.h>
#include "G4ios.hh"

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"


int main(int argc, char* argv[])
{

  G4UIExecutive *ui = nullptr;
  if (argc == 1)
  {
    ui = new G4UIExecutive(argc, argv);
  }
  G4RunManager * theRunManager = new G4RunManager;

  G4int precision = 8;
  G4SteppingVerbose::UseBestUnit(precision);

  G4VModularPhysicsList *physicsList = new FTFP_BERT;
  physicsList->SetVerboseLevel(0);
  theRunManager->SetNumberOfThreads(16);
  physicsList->RegisterPhysics(new G4StepLimiterPhysics());
  theRunManager->SetUserInitialization(physicsList);

  theRunManager->SetUserInitialization(new MDT::DetectorConstruction());
  theRunManager->SetUserInitialization(new MDT::ActionInitialization());
  theRunManager->SetUserAction(new PrimaryGeneratorAction("setup.file"));

  theRunManager->Initialize();

  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager* UI = G4UImanager::GetUIpointer();

  if (argc > 1)
  {
    UI->ApplyCommand("/control/execute " + G4String(argv[1]));
  }
  else
  {
    UI->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  delete theRunManager;
  delete visManager;
  return EXIT_SUCCESS;
}
