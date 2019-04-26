#include "GasModelParametersMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4UIparameter.hh"
#include "GasModelParameters.hh"
#include "DegradModel.hh"

#include "G4Tokenizer.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GasModelParametersMessenger::GasModelParametersMessenger(GasModelParameters* gm)
    : fGasModelParameters(gm) {
  GasModelParametersDir = new G4UIdirectory("/gasModelParameters/");
  GasModelParametersDir->SetGuidance("GasModelParameters specific controls");
  DegradDir = new G4UIdirectory("/gasModelParameters/degrad/");
  DegradDir->SetGuidance("Degrad specific controls");

  thermalEnergyCmd = new G4UIcmdWithADoubleAndUnit("/gasModelParameters/degrad/thermalenergy",this);
  thermalEnergyCmd->SetGuidance("Set the thermal energy to be used by degrad");
  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GasModelParametersMessenger::~GasModelParametersMessenger() {
  delete GasModelParametersDir;
  delete DegradDir;
  delete thermalEnergyCmd;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GasModelParametersMessenger::SetNewValue(G4UIcommand* command, G4String newValues) {
    if(command == thermalEnergyCmd){
      fGasModelParameters->SetThermalEnergy(thermalEnergyCmd->GetNewDoubleValue(newValues));
    }

}
