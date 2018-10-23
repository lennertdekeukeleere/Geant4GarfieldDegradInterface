#include "DegradMessenger.hh"

#include "DegradModel.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DegradMessenger::DegradMessenger(DegradModel* dm)
    : fDegradModel(dm) {
  DegradDir = new G4UIdirectory("/Degrad/");
  DegradDir->SetGuidance("Degrad specific controls");

  
  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DegradMessenger::~DegradMessenger() {
  delete DegradDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DegradMessenger::SetNewValue(G4UIcommand* command, G4String newValues) {
  
}
