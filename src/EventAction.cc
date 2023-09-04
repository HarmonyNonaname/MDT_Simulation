#include "EventAction.hh"
#include "RunAction.hh"
#include "EventMessenger.hh"
#include "TubeHit.hh"
#include "ScinHit.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsMap.hh"
#include "G4SystemOfUnits.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleDefinition.hh"

#include <fstream>
#include <unistd.h>

#include "globals.hh"


namespace MDT
{

EventAction::EventAction(RunAction* runAction)
: fRunAction(runAction)
{
    fMessenger = new EventMessenger(this);
}

EventAction::~EventAction()
{
    delete fMessenger;
}

void EventAction::BeginOfEventAction(const G4Event* event)
{
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 8; k++){
                fCollID_Tube[i][j][k] = -1;
            }
        }
    }
}

void EventAction::EndOfEventAction(const G4Event* event)
{

    G4HCofThisEvent* HCofEvent = event->GetHCofThisEvent();
    if(!HCofEvent) return;

    #ifdef G4MULTITHREADED
        static G4Mutex MDTMutex = G4MUTEX_INITIALIZER;
        G4AutoLock al(&MDTMutex);
    #endif

    static std::ofstream output_xyz("hit_position.dat", std::ios_base::out); // Record x y and z position of hitting point
    static std::ofstream output_index("hit_index.dat", std::ios_base::out); // record hit time and which tube be hitted
    static std::ofstream output_angle("primary_angle.dat", std::ios_base::out); // record whether be accepted and primary angle

    /*Get Primary Particle Information*/
    auto fprimaryVertex = event->GetPrimaryVertex();
    auto fprimaryParticle = fprimaryVertex->GetPrimary();
    auto fprimaryParticleDefinition = fprimaryParticle->GetParticleDefinition();

    output_angle << fprimaryParticle->GetPx() << " " << fprimaryParticle->GetPy() << " " << fprimaryParticle->GetPz() << " ";
    output_xyz<<event->GetEventID()<<" "<<fprimaryParticleDefinition->GetParticleName()<<" "<<fprimaryParticle->GetKineticEnergy()<<G4endl;
    output_index<<event->GetEventID()<<" "<<fprimaryParticleDefinition->GetParticleName()<<" "<<fprimaryParticle->GetKineticEnergy()<<G4endl;

    fCollID_Scin1 = G4SDManager::GetSDMpointer()->GetCollectionID("HCScin1");
    fCollID_Scin2 = G4SDManager::GetSDMpointer()->GetCollectionID("HCScin2");
    fCollID_Scin3 = G4SDManager::GetSDMpointer()->GetCollectionID("HCScin3");
    ScinHitsCollection* hitsCollection_Scin1 = (ScinHitsCollection*)(HCofEvent->GetHC(fCollID_Scin1));
    ScinHitsCollection* hitsCollection_Scin2 = (ScinHitsCollection*)(HCofEvent->GetHC(fCollID_Scin2));
    ScinHitsCollection* hitsCollection_Scin3 = (ScinHitsCollection*)(HCofEvent->GetHC(fCollID_Scin3));

    //G4cout << hitsCollection_Scin1->entries() <<" "<< hitsCollection_Scin2->entries() << " "<< hitsCollection_Scin3->entries() <<G4endl;

    /*Condition: whether pass through the Scintillators*/
    if(hitsCollection_Scin1->entries() < 1 || (hitsCollection_Scin2->entries() < 1 && hitsCollection_Scin3->entries() < 1))
    {
        output_xyz<<G4endl;
        output_index<<G4endl;
        output_angle << "0"  << G4endl;
        return;
    }

    /* Condition: whther the energy deposited in scintillator bigger than the threshold*/
    /* Set 0 for threshold will omit this block*/
    if(GetScinThreshold() != 0 ){
        G4double Scin1_edep =0.;
        G4double Scin2_edep =0.;
        G4double Scin3_edep =0.;
        for(int i = 0; i < hitsCollection_Scin1->entries(); i++){
            ScinHit* hit = (*hitsCollection_Scin1)[i];
            Scin1_edep += hit->GeteDep();
        }
        for(int i = 0; i < hitsCollection_Scin2->entries(); i++){
            ScinHit* hit = (*hitsCollection_Scin2)[i];
            Scin2_edep += hit->GeteDep();
        }
        for(int i = 0; i < hitsCollection_Scin3->entries(); i++){
            ScinHit* hit = (*hitsCollection_Scin3)[i];
            Scin3_edep += hit->GeteDep();
        }
        if(Scin1_edep < GetScinThreshold() || (Scin2_edep < GetScinThreshold() && Scin3_edep < GetScinThreshold()))
        {
            output_xyz<<G4endl;
            output_index<<G4endl;
            output_angle << "0"  << G4endl;
            return;
        }
    }

    output_angle << "1"  << G4endl;

    G4String HCname = "";
    TubeHitsCollection* hitsCollection = 0;

    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 8; k++){
                if ( fCollID_Tube[i][j][k] < 0 ){

                    HCname = "HCTube_"+std::to_string(i)+"_"+std::to_string(j)+"_"+std::to_string(k);
                    fCollID_Tube[i][j][k] = G4SDManager::GetSDMpointer()->GetCollectionID(HCname);
                }
                //G4cout<<"Hit Collection ID is: "<<fCollID_Tube[i][j][k]<<G4endl;

                hitsCollection = (TubeHitsCollection*)(HCofEvent->GetHC(fCollID_Tube[i][j][k]));

                if(hitsCollection)
                {
                    int numberHits = hitsCollection->entries();
                    G4double Tube_edep =0.;
                    for(int l = 0; l < numberHits; l++){
                        TubeHit* hit = (*hitsCollection)[l];
                        Tube_edep += hit->GeteDep();
                    }
                    if(Tube_edep < GetTubeThreshold()) { continue; }
                    for(int l = 0; l < numberHits; l++){
                        TubeHit* hit = (*hitsCollection)[l];
                        output_xyz<<hit->GetPos().getX()<<" "<<hit->GetPos().getY()<<" "<<hit->GetPos().getZ()<<" ";
                        output_index<<hit->GetTime()<<" "<<hit->GeteDep()<<" "<<hit->GetLogV()->GetName()<<" ";

                        output_xyz<<G4endl;
                        output_index<<G4endl;
                    }
                }
            }
        }
    }

    output_xyz<<G4endl;
    output_index<<G4endl;

    #if defined (_WIN64) || defined (_WIN32) || defined (WIN32)
    #else
        sync();
    #endif

}

void EventAction::SetScinThreshold(G4double energy)
{
    ScinThreshold = energy;
}

void EventAction::SetTubeThreshold(G4double energy)
{
    TubeThreshold = energy;
}

}
