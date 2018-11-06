#include "GasBoxSD.hh"
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

GasBoxSD::GasBoxSD(G4String name) : G4VSensitiveDetector(name), fGasBoxHitsCollection(NULL),
    fXenonHitsCollection(NULL), fGarfieldExcitationHitsCollection(NULL), fDriftLineHitsCollection(NULL){
    collectionName.insert("GBHC");
    collectionName.insert("XHC");
    collectionName.insert("GEHC");
    collectionName.insert("DLHC");
    
    GBHCID=-1;
    XHCID=-1;
    GEHCID=-1;
    DLHCID=-1;
}

GasBoxSD::~GasBoxSD(){}


void GasBoxSD::Initialize(G4HCofThisEvent * HCE){
    fGasBoxHitsCollection = new GasBoxHitsCollection(SensitiveDetectorName, collectionName[0]);
    fXenonHitsCollection = new XenonHitsCollection(SensitiveDetectorName, collectionName[1]);
    fGarfieldExcitationHitsCollection = new GarfieldExcitationHitsCollection(SensitiveDetectorName, collectionName[2]);
    fDriftLineHitsCollection = new DriftLineHitsCollection(SensitiveDetectorName, collectionName[3]);
    if(GBHCID==-1){
        GBHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
        XHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[1]);
        GEHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[2]);
        DLHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[3]);
    }
    HCE->AddHitsCollection(GBHCID,fGasBoxHitsCollection);
    HCE->AddHitsCollection(XHCID,fXenonHitsCollection);
    HCE->AddHitsCollection(GEHCID,fGarfieldExcitationHitsCollection);
    HCE->AddHitsCollection(DLHCID,fDriftLineHitsCollection);

    G4cout << "GasBoxSD Intialized!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << G4endl;
}

G4bool GasBoxSD::ProcessHits(G4Step* aStep, G4TouchableHistory* hist){
    G4Track* aTrack = aStep->GetTrack();
    G4StepPoint* thePostPoint = aStep->GetPostStepPoint();

    G4cout << "Hit!!" << G4endl;

    if(/*aTrack->GetTrackStatus() == fStopAndKill &&*/
       aTrack->GetDefinition()->GetParticleName() == "pi+"){
        G4cout << "Hit!!" << G4endl;
        GasBoxHit* hit = new GasBoxHit();
        G4ThreeVector pos = thePostPoint->GetPosition();
        hit->SetPos(pos);
        hit->SetTime(aTrack->GetGlobalTime());
        fGasBoxHitsCollection->insert(hit);
        return true;
    }

    return false;
    
    
}
