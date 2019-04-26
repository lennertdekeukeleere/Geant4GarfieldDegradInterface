#include "SiliconSD.hh"
#include "G4Region.hh"
#include "G4String.hh"
#include "G4Track.hh"
#include "GasBoxHit.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4VProcess.hh"
#include "DetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"

SiliconSD::SiliconSD(G4String name) : G4VSensitiveDetector(name), fSiliconHitsCollection(NULL){
    collectionName.insert("SHC");
    SHCID=-1;
}

SiliconSD::~SiliconSD(){}


void SiliconSD::Initialize(G4HCofThisEvent * HCE){
    fSiliconHitsCollection = new SiliconHitsCollection(SensitiveDetectorName, collectionName[0]);
    if(SHCID==-1){
        SHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    HCE->AddHitsCollection(SHCID,fSiliconHitsCollection);

    G4cout << "SiliconSD Intialized!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << G4endl;
}

G4bool SiliconSD::ProcessHits(G4Step* aStep, G4TouchableHistory* hist){
    G4Track* aTrack = aStep->GetTrack();
    G4StepPoint* thePostPoint = aStep->GetPostStepPoint();

    if(aStep->IsFirstStepInVolume()){
        G4cout << "Silicon Wall Hit!!" << G4endl;
        G4cout << "Particle ID: " << aTrack->GetTrackID() << G4endl;
        G4cout << "Energy loss through gas: " << aTrack->GetVertexKineticEnergy() - aTrack->GetKineticEnergy() << G4endl;
        return true;
    }
    return false;      
}

void SiliconSD::EndOfEvent (G4HCofThisEvent * hce){
   
}