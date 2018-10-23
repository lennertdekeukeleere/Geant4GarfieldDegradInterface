#include "SteppingAction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4Step.hh"
#include "DetectorConstruction.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4StepPoint.hh"
#include "G4SDManager.hh"
#include "GasBoxSD.hh"



SteppingAction::SteppingAction(){

}


void SteppingAction::UserSteppingAction(const G4Step *aStep) {

    
    G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
    G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
    G4VPhysicalVolume *pVol =
      thePrePoint->GetTouchableHandle()->GetVolume();
    G4VPhysicalVolume *pVolPost =
    thePostPoint->GetTouchableHandle()->GetVolume();
	
}
