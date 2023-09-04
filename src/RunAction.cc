#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "TubeHit.hh"

#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Run.hh"
#include "G4ios.hh"
#include "G4THitsMap.hh"
#include "G4AnalysisManager.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "globals.hh"

namespace MDT
{

RunAction::RunAction()
{
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}

RunAction::~RunAction()
{}

void RunAction::BeginOfRunAction(const G4Run*)
{

    G4RunManager::GetRunManager()->SetRandomNumberStore(false);
    G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
    accumulableManager->Reset();

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile();

}

void RunAction::EndOfRunAction(const G4Run* run)
{

}

}
