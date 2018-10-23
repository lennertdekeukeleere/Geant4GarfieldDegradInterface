#include "HeedMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HeedMessenger::HeedMessenger(HeedPhysics* gp)
    : HeedPhysics(gp) {
  HeedDir = new G4UIdirectory("/Heed/");
  HeedDir->SetGuidance("Heed specific controls");

  
  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HeedMessenger::~HeedMessenger() {
  delete HeedDir;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HeedMessenger::SetNewValue(G4UIcommand* command, G4String newValues) {
  
}
