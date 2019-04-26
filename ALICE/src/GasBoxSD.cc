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
#include "G4VVisManager.hh"
#include "G4Polyline.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

GasBoxSD::GasBoxSD(G4String name) : G4VSensitiveDetector(name), fGasBoxHitsCollection(NULL){
    collectionName.insert("GBHC");
    GBHCID=-1;
}

GasBoxSD::~GasBoxSD(){}


void GasBoxSD::Initialize(G4HCofThisEvent * HCE){
    fGasBoxHitsCollection = new GasBoxHitsCollection(SensitiveDetectorName, collectionName[0]);
    if(GBHCID==-1){
        GBHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    HCE->AddHitsCollection(GBHCID,fGasBoxHitsCollection);
    G4cout << "GasBoxSD Intialized!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << G4endl;
}

G4bool GasBoxSD::ProcessHits(G4Step* aStep, G4TouchableHistory* hist){
    G4Track* aTrack = aStep->GetTrack();

    if(aTrack->GetDefinition()->GetParticleName() == "e-"){
        G4cout << "GasBox Hit!!" << G4endl;
        G4cout << "Particle ID: " << aTrack->GetTrackID() << G4endl;
        G4cout << "Energy electron: " << aTrack->GetKineticEnergy() << G4endl;
        return true;
    }

    return false;
    
    
}

void GasBoxSD::EndOfEvent (G4HCofThisEvent * hce){
    auto HC = static_cast<GasBoxHitsCollection*>(hce->GetHC(GBHCID));
    int entries = HC->entries();
    G4cout << "Number of Electrons: " << entries << G4endl;
    for(int i=0;i<entries;i++){
        auto hit = (*HC)[i];
        G4cout << hit->GetPos() << " " << hit->GetTime() << G4endl;
    }
    DrawAll();
}

void GasBoxSD::DrawAll(){}
