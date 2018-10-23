//Runaction
#include "RunAction.hh"
#include "G4Run.hh"
#include "Randomize.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunMessenger.hh"
#include "Analysis.hh"
#include "EventAction.hh"
#include "GarfieldModel.hh"
#include "GarfieldPhysics.hh"
#include "GasBoxSD.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

RunAction::RunAction(DetectorConstruction* DC, PrimaryGeneratorAction* pg,
                     PhysicsList* phys, EventAction* evact):evAct(evact) {
  myDC = DC;
  primary = pg;
  outputFileName = "results.root";
  messenger = new RunMessenger(this);
  
  G4cout << "Creating AnalysisManager" << G4endl;
  auto analysisManager = G4AnalysisManager::Instance();
//  analysisManager->SetNtupleMerging(true,0,0,10000000);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);  
  analysisManager->SetFileName(outputFileName); 
  analysisManager->SetHistoDirectoryName("histo");
  analysisManager->SetNtupleDirectoryName("ntuple");

  G4double scintWidth = myDC->GetScintWidth();
  analysisManager->CreateH1("LG1", "Photons Lightguide 1",10000, 0, 10000);
  analysisManager->CreateH1("SC1E", "Energy deposit in upper scintillator", 30000, 0., 30.);
  analysisManager->CreateH1("GB1", "Number of secondaries created in gas", 1000, 0, 1000);
  analysisManager->CreateH1("trackLength", "Total tracklength of optical photons",1000, 0, 10000);
  analysisManager->CreateH1("trackLengthSC", "tracklength of optical photons in scintillator",500, 0, 5000.);
  analysisManager->CreateH1("trackLengthLG", "tracklength of optical photons in lightguides", 700, 0., 7000.);
  analysisManager->CreateH1("trackLengthLGP", "tracklength of optical photons in lightguidepipes", 300, 0, 3000.);
  analysisManager->CreateH1("secEnergyDistFirst", "Energy distribution of the first generation secondary electrons", 200, 0, 2.);
  analysisManager->CreateH1("secEnergyDistSecond", "Energy distribution of the second generation secondary electrons", 200, 0, 2.);
  analysisManager->CreateH1("photonEnergies", "Energy distribution of the scintillator photons", 200, 0, 10);
  analysisManager->CreateH1("CN", "copy nr", 80, 0, 80);
  analysisManager->CreateH2("SC1P", "Position of entry upper scintillator", 1000, -scintWidth*0.5, scintWidth*0.5,1000,-scintWidth*0.5,scintWidth*0.5);
  
  
  analysisManager->SetH1Activation(0, myDC->GetBuildUpperScint() || myDC->GetBuildLowerScint());
  analysisManager->SetH1Activation(1, myDC->GetBuildUpperScint() || myDC->GetBuildLowerScint());
  analysisManager->SetH2Activation(0, myDC->GetBuildUpperScint() || myDC->GetBuildLowerScint());
    
  analysisManager->CreateNtuple("GasBox","Information secondary electrons");
  analysisManager->CreateNtupleIColumn("nSecondaries");
  analysisManager->CreateNtupleDColumn("Xpos", evAct->GetXPosVector());
  analysisManager->CreateNtupleDColumn("Ypos", evAct->GetYPosVector());
  analysisManager->CreateNtupleDColumn("Zpos", evAct->GetZPosVector());
  analysisManager->CreateNtupleDColumn("Ekin", evAct->GetEnergyVector());
  analysisManager->CreateNtupleDColumn("time", evAct->GetTimeVector());
  analysisManager->CreateNtupleIColumn("ID", evAct->GetIDVector());
  analysisManager->CreateNtupleIColumn("parentID", evAct->GetparentIDVector());
  analysisManager->CreateNtupleIColumn("process", evAct->GetProcessVector());
  analysisManager->CreateNtupleDColumn("Edep");
  analysisManager->CreateNtupleDColumn("Efinal");
  
  analysisManager->FinishNtuple();
  
  analysisManager->CreateNtuple("CELL", "Cell signal parameters");
  analysisManager->CreateNtupleDColumn("POCA", evAct->GetPOCAVector());
  analysisManager->CreateNtupleDColumn("Zentry",evAct->GetZentryVector());
  analysisManager->CreateNtupleDColumn("ADC",evAct->GetADCVector());
  analysisManager->CreateNtupleDColumn("TDC",evAct->GetTDCVector());
  analysisManager->CreateNtupleDColumn("RiseTime",evAct->GetRiseTimeVector());
  analysisManager->CreateNtupleDColumn("Naval",evAct->GetAvalancheSizeVector());
  analysisManager->FinishNtuple();
  
  analysisManager->CreateNtuple("Detector","Scintillator and Lightguide");
  analysisManager->CreateNtupleDColumn("Xpos");
  analysisManager->CreateNtupleDColumn("Ypos");
  analysisManager->CreateNtupleDColumn("Zpos");
  analysisManager->CreateNtupleDColumn("Edep");
  analysisManager->CreateNtupleDColumn("TrackLength");
  analysisManager->CreateNtupleIColumn("NCreatedPhotons");
  analysisManager->CreateNtupleIColumn("NphotonsLG1");
  analysisManager->CreateNtupleIColumn("NphotonsLG2");
  analysisManager->CreateNtupleIColumn("NphotonsLG3");
  analysisManager->CreateNtupleIColumn("NphotonsLG4");
  analysisManager->CreateNtupleIColumn("NphotonsLG1Pipe");
  analysisManager->CreateNtupleIColumn("NphotonsLG2Pipe");
  analysisManager->CreateNtupleIColumn("NphotonsLG3Pipe");
  analysisManager->CreateNtupleIColumn("NphotonsLG4Pipe");
  analysisManager->CreateNtupleIColumn("NDetphotons1");
  analysisManager->CreateNtupleIColumn("NDetphotons2");
  analysisManager->CreateNtupleIColumn("NDetphotons3");
  analysisManager->CreateNtupleIColumn("NDetphotons4");
  analysisManager->CreateNtupleIColumn("NDetphotons5");
  analysisManager->CreateNtupleIColumn("NDetphotons6");
  analysisManager->CreateNtupleIColumn("NDetphotons7");
  analysisManager->CreateNtupleIColumn("electronHits");
  analysisManager->CreateNtupleIColumn("gammaHits");
  analysisManager->CreateNtupleIColumn("muonHits");
  analysisManager->CreateNtupleIColumn("compton");
  analysisManager->CreateNtupleIColumn("ID");
  analysisManager->CreateNtupleDColumn("initX");
  analysisManager->CreateNtupleDColumn("initY");
  analysisManager->CreateNtupleDColumn("initZ");
  analysisManager->CreateNtupleDColumn("initE");
  analysisManager->FinishNtuple();
  
  analysisManager->CreateNtuple("Primary","Information for the primary particle");
  analysisManager->CreateNtupleDColumn("Xpos");
  analysisManager->CreateNtupleDColumn("Ypos");
  analysisManager->CreateNtupleDColumn("Zpos");
  analysisManager->CreateNtupleDColumn("Xmom");
  analysisManager->CreateNtupleDColumn("Ymom");
  analysisManager->CreateNtupleDColumn("Zmom");
  analysisManager->CreateNtupleDColumn("Energy");
  analysisManager->FinishNtuple();
  
  analysisManager->CreateNtuple("Attributes","Information about the measurement configuration");
  analysisManager->CreateNtupleDColumn("sourcePosX");
  analysisManager->CreateNtupleDColumn("sourcePosZ");
  analysisManager->CreateNtupleDColumn("slitUpperX");
  analysisManager->CreateNtupleDColumn("slitUpperWidth");
  analysisManager->CreateNtupleDColumn("slitLowerX");
  analysisManager->CreateNtupleDColumn("slitLowerWidth");
  analysisManager->FinishNtuple();
  
  analysisManager->SetNtupleActivation(false);

  G4cout << "Creating RunAction" << G4endl;
}

RunAction::~RunAction() { 
	G4cout << "Deleting RunAction" << G4endl;
//	delete G4AnalysisManager::Instance();  
}


void RunAction::BeginOfRunAction(const G4Run* aRun) {
  G4Random::showEngineStatus();

  G4cout << "Starting run " << aRun->GetRunID() << G4endl;
  time_t currentTime;
  tm* ptm;
  time(&currentTime);
  ptm = localtime(&currentTime);
  G4cout << "Time: " << asctime(ptm) << G4endl;

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile();
  
  if(G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::sequentialRM || 
		G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::workerRM){
		evAct->Initialize();
    G4ThreeVector pos = myDC->GetGunPosition();
    G4String slitName = myDC->GetSlitName();
  //  G4String slitType = myDC->GetSlitType();
    G4bool slitAlongWires = myDC->GetSlitAlongWires();
    G4double slitMetalLayerOpening = myDC->GetSlitOpening();
    G4double slitUpperX;
    if(slitAlongWires){
      if(slitName == "Single0"){
        slitUpperX = 0.;
      }
      else if(slitName == "Single+"){
        slitUpperX = 20.;
      }
      else if(slitName == "Single-"){
        slitUpperX = -20.;
      }
      else if(slitName == "Single++"){
        slitUpperX = 40.;
      }
      else if(slitName == "Single--"){
        slitUpperX = 40.;
      }
      else{
        slitUpperX = -1;
      }
    }
    else {
      slitUpperX=-1;
    }

    analysisManager->FillNtupleDColumn(4,0,pos.x());
    analysisManager->FillNtupleDColumn(4,1,pos.z());
    analysisManager->FillNtupleDColumn(4,2,slitUpperX);
    analysisManager->FillNtupleDColumn(4,3,slitMetalLayerOpening);
    analysisManager->FillNtupleDColumn(4,4,-1);
    analysisManager->FillNtupleDColumn(4,5,-1);
    analysisManager->AddNtupleRow(4);
  }
 
   myDC->SetBoxBoundaryVolumes();
  
}

void RunAction::EndOfRunAction(const G4Run* aRun) {
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  G4cout << "End of run OK!" << G4endl;
  time_t currentTime;
  tm* ptm;
  time(&currentTime);
  ptm = localtime(&currentTime);
  G4cout << "Simulation finished." << G4endl << "Time: " << asctime(ptm)
         << G4endl;
  G4Random::showEngineStatus();
}
