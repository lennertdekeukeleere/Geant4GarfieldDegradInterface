#include "SteppingAction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4Step.hh"
#include "DetectorConstruction.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4StepPoint.hh"
#include "G4SDManager.hh"
#include "LightGuideSD.hh"
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
	
    G4VPhysicalVolume* scint = myDC->GetUpperScintillator();
    std::vector<G4VPhysicalVolume *> lightguides = myDC->GetLightGuides();
    std::vector<G4VPhysicalVolume *> lightguidepipes = myDC->GetLightGuidePipes();
    std::vector<G4VPhysicalVolume*> PMTs = myDC->GetPMTs();

	G4Track* aTrack = aStep->GetTrack();
    

    if ( aTrack->GetCurrentStepNumber() == 1 ) fExpectedNextStatus = Undefined;
    
    if(!pVolPost){//out of world
        fExpectedNextStatus=Undefined;
        return;
    }
    
    if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition()){
     /*   if(pVol && pVolPost){
            G4cout << pVol->GetName() << " " << pVolPost->GetName() << G4endl;
            G4cout << "Position: " << aTrack->GetPosition() << G4endl;
            G4cout << "ID: " << aTrack->GetTrackID() << G4endl;
    }*/
    /*    
        int i;
        for(i=0; i<=Photons.size();i++){
            if(i==Photons.size()) break;
            else if(Photons[i].ID == aTrack->GetTrackID()){
//                G4cout << "Track Found !!!!!" << G4endl;
                if(pVol == scint) {
                    Photons[i].trackLengthSC += aStep->GetStepLength();
                    break;
                }
                else{
                    for(int j=0; j<lightguides.size();j++){
                        if(pVol == lightguides[j]) {
                            Photons[i].trackLengthLG += aStep->GetStepLength();
                            break;
                        }
                        else if(lightguidepipes.size()!=0 && pVol == lightguidepipes[j]){
                            Photons[i].trackLengthLGP += aStep->GetStepLength();
                            break;
                        }
                    }
                    break;
                }
            }
        }
        if(i==Photons.size()){
 //           G4cout << "Track Added !!!!!" << G4endl;
            photon ph;
            ph.ID=aTrack->GetTrackID();
            if(pVol == scint) {
                    ph.trackLengthSC = aStep->GetStepLength();
                    ph.trackLengthLG = 0.;
                    ph.trackLengthLGP = 0.;
                }
            else{
                for(int j=0; j<lightguides.size();j++){
                    if(pVol == lightguides[j]) {
                        ph.trackLengthLG = aStep->GetStepLength();
                        ph.trackLengthSC = 0.;
                        ph.trackLengthLGP = 0.;
                        break;
                    }
                    else if(pVol == lightguidepipes[j]){
                        ph.trackLengthLGP = aStep->GetStepLength();
                        ph.trackLengthSC = 0.;
                        ph.trackLengthLG = 0.;
                        break;
                    }
                }
            }
            Photons.push_back(ph);
        }
        */
 //       G4cout << "looking for type of boundary process" << G4endl;
        G4OpBoundaryProcessStatus boundaryStatus=Undefined;
        static G4ThreadLocal G4OpBoundaryProcess* boundary=NULL;

        //find the boundary process only once
        if(!boundary){
            G4ProcessManager* pm
              = aStep->GetTrack()->GetDefinition()->GetProcessManager();
            G4int nprocesses = pm->GetProcessListLength();
            G4ProcessVector* pv = pm->GetProcessList();
            G4int i;
            for( i=0;i<nprocesses;i++){
              if((*pv)[i]->GetProcessName()=="OpBoundary"){
                boundary = (G4OpBoundaryProcess*)(*pv)[i];
                break;
              }
            }
        }
        boundaryStatus=boundary->GetStatus();
//        G4cout << "ExpectedNextStatus: " << fExpectedNextStatus << " boundaryStatus: " << boundaryStatus << G4endl;
        if(thePostPoint->GetStepStatus()==fGeomBoundary){
            if(fExpectedNextStatus==StepTooSmall){
                if(boundaryStatus!=StepTooSmall){
                    G4ExceptionDescription ed;
                    ed << "SteppingAction::UserSteppingAction(): "
                    << "No reallocation step after reflection!"
                    << G4endl;
                    G4Exception("SteppingAction::UserSteppingAction()", "SteppingAction",
                    JustWarning ,ed,
                    "Something is wrong with the surface normal or geometry");
                }
            }
            fExpectedNextStatus=Undefined;
            switch(boundaryStatus){
            case Detection:
                //Triger sensitive detector manually since photon is
                //absorbed but status was Detection
                
                if(PMTs.size()==0){
                    for(int j=0; j<lightguidepipes.size();j++){
                        if(pVol == lightguidepipes[j]) {
//                            G4SDManager* SDman = G4SDManager::GetSDMpointer();
//                            G4String sdName=SDnames[j];
//                            LightGuideSD* lightGuidePipeSD = (LightGuideSD*)SDman->FindSensitiveDetector(sdName);
                            LightGuideSD* lightGuidePipeSD = (LightGuideSD*)(thePrePoint->GetSensitiveDetector());
                            if(lightGuidePipeSD) lightGuidePipeSD->AddDetection();
                            break;
                        }
                    }
                }
                else{
                    for(int j=0; j<PMTs.size();j++){
                        if(pVolPost == PMTs[j]) {
//                            G4SDManager* SDman = G4SDManager::GetSDMpointer();
//                            G4String sdName="minib/myLightGuide1SD";
//                            LightGuideSD* lightGuide1SD = (LightGuideSD*)SDman->FindSensitiveDetector(sdName);
                            LightGuideSD* lightGuide1SD = (LightGuideSD*)(thePrePoint->GetSensitiveDetector());
                            if(lightGuide1SD) lightGuide1SD->AddDetection(j);
                            break;
                        }
                    }
                }
                break;
            case FresnelReflection:
            case TotalInternalReflection:
            case LambertianReflection:
            case LobeReflection:
            case SpikeReflection:
            case BackScattering:
                fExpectedNextStatus=StepTooSmall;
                break;
            default:
                break;
            }
        }

    }
    /*G4String pName = "";
    const G4VProcess* p = aTrack->GetCreatorProcess();
    
    if (p) {
        if(!fGasBoxSD) {
            auto sdManager = G4SDManager::GetSDMpointer();
            fGasBoxSD=static_cast<GasBoxSD*>(sdManager->FindSensitiveDetector("minib/myGasBoxSD"));
        }
        pName = p->GetProcessName();
        if(aTrack->GetDefinition()->GetParticleName() == "e-" && aTrack->GetTrackID()>1 && aTrack->GetVolume()->GetName() == "GasBox" && aTrack->GetCurrentStepNumber()==1){
            G4ThreeVector pos = aTrack->GetPosition();

            G4ThreeVector mom = aTrack->GetMomentumDirection();

            G4double energy = aTrack->GetKineticEnergy();
            G4double time = aTrack->GetGlobalTime();
            
            G4int id = aTrack->GetTrackID();
            G4int parentID = aTrack->GetParentID();
            G4int process;  
            if(pName == "eIoni") process = 1;
            else process = 0;

            if(fGasBoxSD) fGasBoxSD->AddSecondary(pos,energy,time,id,parentID,process);
        }
        
    }
    */
}

/*    if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition()){
        G4OpBoundaryProcess* boundary = (G4OpBoundaryProcess*)(aStep->GetPostStepPoint()->GetProcessDefinedStep());
        G4cout << "Status: " << boundary->GetStatus() << G4endl;
        if(pVol && pVolPost){
            G4cout << pVol->GetName() << " " << pVolPost->GetName() << G4endl;
            G4cout << "Position: " << aTrack->GetPosition() << G4endl;
        }
        G4cout << " " << G4endl;
        if(boundary->GetStatus()==Detection){
            G4cout << "Status: " << boundary->GetStatus() << G4endl;
            G4cout << pVol->GetName() << G4endl;
            G4cout << pVolPost->GetName() << G4endl;
        }
    }
*/    


/*   if(aTrack->GetCurrentStepNumber()<=10 && aTrack->GetTrackID()==1)
G4cout << pVol->GetName() << G4endl;
if(aTrack->GetDefinition()->GetParticleName() == "mu+" && aStep->GetPostStepPoint()->GetStepStatus()==fGeomBoundary){
    std::vector<G4VPhysicalVolume *> boxBoundary = myDC->GetBoxBoundaryVolumes();
    G4VPhysicalVolume* vol = aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
    if(vol){
        //            G4cout << "Going out: " << vol->GetName() << G4endl;
        for(int i=0;i<boxBoundary.size();i++){
            if(vol==boxBoundary[i]){
                aStep->GetTrack()->SetTrackStatus(fStopAndKill);
                return;
            }
        }
    }
}
if(pVol != myDC->GetUpperScintillator() && pVol != myDC->GetLowerScintillator() && pVol != myDC->GetGasBoxVolume() && pVol->GetName() != "Cell"){
    if(aTrack->GetTrackID()>1)
        aTrack->SetTrackStatus(fStopAndKill);
}
}
*/
