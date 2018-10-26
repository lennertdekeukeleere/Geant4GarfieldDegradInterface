#include "PhysicsListMessenger.hh"
#include "PhysicsList.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* pPhys)
    : pPhysicsList(pPhys) {
  physDir = new G4UIdirectory("/InterfaceExample/phys/");
  physDir->SetGuidance("InterfaceExample physics list commands");

  gammaCutCmd = new G4UIcmdWithADoubleAndUnit("/InterfaceExample/phys/setGCut", this);
  gammaCutCmd->SetGuidance("Set gamma cut.");
  gammaCutCmd->SetParameterName("Gcut", false);
  gammaCutCmd->SetUnitCategory("Length");
  gammaCutCmd->SetRange("Gcut>0.0");
  gammaCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  electCutCmd = new G4UIcmdWithADoubleAndUnit("/InterfaceExample/phys/setECut", this);
  electCutCmd->SetGuidance("Set electron cut.");
  electCutCmd->SetParameterName("Ecut", false);
  electCutCmd->SetUnitCategory("Length");
  electCutCmd->SetRange("Ecut>0.0");
  electCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  protoCutCmd = new G4UIcmdWithADoubleAndUnit("/InterfaceExample/phys/setPCut", this);
  protoCutCmd->SetGuidance("Set positron cut.");
  protoCutCmd->SetParameterName("Pcut", false);
  protoCutCmd->SetUnitCategory("Length");
  protoCutCmd->SetRange("Pcut>0.0");
  protoCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  allCutCmd = new G4UIcmdWithADoubleAndUnit("/InterfaceExample/phys/setCuts", this);
  allCutCmd->SetGuidance("Set cut for all.");
  allCutCmd->SetParameterName("cut", false);
  allCutCmd->SetUnitCategory("Length");
  allCutCmd->SetRange("cut>0.0");
  allCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  pListCmd = new G4UIcmdWithAString("/InterfaceExample/phys/ReplacePhysics", this);
  pListCmd->SetGuidance("Add modula physics list.");
  pListCmd->SetParameterName("PList", false);
  pListCmd->AvailableForStates(G4State_PreInit);
  pListCmd->SetCandidates(
      "local emstandard_opt0 emstandard_opt1 emstandard_opt2 emstandard_opt3 "
      "standardSS standardGS standardWVI emlivermore empenelope");

  lowLimitECmd = new G4UIcmdWithADoubleAndUnit("/InterfaceExample/phys/setLowLimitE", this);
  lowLimitECmd->SetGuidance("Set lower limit for production energy");
  lowLimitECmd->SetParameterName("Energy", false);
  lowLimitECmd->SetDefaultUnit("eV");
  lowLimitECmd->SetUnitCandidates("eV keV MeV GeV TeV");
//  lowLimitECmd->SetRange("limit>10.0");
  lowLimitECmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::~PhysicsListMessenger() {
  delete gammaCutCmd;
  delete electCutCmd;
  delete protoCutCmd;
  delete allCutCmd;
  delete pListCmd;
  delete lowLimitECmd;  
  delete physDir;
  G4cout << "Deleting PhysicsListMessenger" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsListMessenger::SetNewValue(G4UIcommand* command,
                                       G4String newValue) {
  if (command == gammaCutCmd) {
    pPhysicsList->SetCutForGamma(gammaCutCmd->GetNewDoubleValue(newValue));
  }

  else if (command == electCutCmd) {
    pPhysicsList->SetCutForElectron(electCutCmd->GetNewDoubleValue(newValue));
  }

  else if (command == protoCutCmd) {
    pPhysicsList->SetCutForPositron(protoCutCmd->GetNewDoubleValue(newValue));
  }

  else if (command == allCutCmd) {
    G4double cut = allCutCmd->GetNewDoubleValue(newValue);
    pPhysicsList->SetCutForGamma(cut);
    pPhysicsList->SetCutForElectron(cut);
    pPhysicsList->SetCutForPositron(cut);
  }

  else if (command == pListCmd) {
    pPhysicsList->ReplacePhysicsList(newValue);
  } 
  else if(command == lowLimitECmd){
	  pPhysicsList->SetLowerProductionLimit(
		lowLimitECmd->GetNewDoubleValue(newValue));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
