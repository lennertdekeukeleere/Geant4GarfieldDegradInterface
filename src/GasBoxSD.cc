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

GasBoxSD::GasBoxSD(G4String name) : G4VSensitiveDetector(name), fGasBoxHitsCollection(NULL){
    collectionName.insert("GBHC");
    HCID=-1;
}

GasBoxSD::~GasBoxSD(){}


void GasBoxSD::Initialize(G4HCofThisEvent * HCE){
    fGasBoxHitsCollection = new GasBoxHitsCollection(SensitiveDetectorName, collectionName[0]);
    if(HCID==-1)
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    HCE->AddHitsCollection(HCID,fGasBoxHitsCollection);
	ef=0;
	edep=0;
    secEnergiesFirstGen.resize(0);
    secEnergiesSecGen.resize(0);
}

G4bool GasBoxSD::ProcessHits(G4Step* aStep, G4TouchableHistory* hist){
    G4Track* aTrack = aStep->GetTrack();
    G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
	edep+=aStep->GetTotalEnergyDeposit();

    

    if(aTrack->GetCurrentStepNumber() == 1 &&
       aTrack->GetDefinition()->GetParticleName() == "e-" && aTrack->GetTrackID()>1){
        GasBoxHit* hit = new GasBoxHit();
        G4ThreeVector pos = thePrePoint->GetPosition();
        hit->SetX(pos.x());
        hit->SetY(pos.y());
        hit->SetZ(pos.z());
        
        hit->SetEkin(thePrePoint->GetKineticEnergy());
        hit->SetTime(aTrack->GetGlobalTime());
        hit->SetID(aTrack->GetTrackID());
        hit->SetParentID(aTrack->GetParentID());
        if(aTrack->GetCreatorProcess()->GetProcessName() == "eIoni")
            hit->SetProcess(1);
        else
            hit->SetProcess(0);
        
        fGasBoxHitsCollection->insert(hit);
        return true;
    }

    return false;
    
    
}

G4bool GasBoxSD::AddSecondary(G4ThreeVector pos, G4double energy, G4double time, G4int id, G4int parentid, G4int process){

    GasBoxHit* hit = new GasBoxHit();
    hit->SetX(pos.x());
    hit->SetY(pos.y());
    hit->SetZ(pos.z());
    
//        G4ThreeVector mom = aTrack->GetMomentumDirection();
//        hit->SetXmomentum(mom.x());
//        hit->SetYmomentum(mom.y());
//        hit->SetZmomentum(mom.z());
    
    hit->SetEkin(energy);
    hit->SetTime(time);
    hit->SetID(id);
    hit->SetParentID(parentid);
    hit->SetProcess(process);
        
    if(fGasBoxHitsCollection) fGasBoxHitsCollection->insert(hit);
}
