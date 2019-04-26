#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* HPGeDet)
    : detector(HPGeDet) {
    miniDir = new G4UIdirectory("/Xenon/");
    miniDir->SetGuidance("Xenon specific controls");

    ////////////////////
    geometryDir = new G4UIdirectory("/Xenon/geometry/");
    geometryDir->SetGuidance("Xenon geometry specific controls");


    ////////////////////
    setGasPressCmd =
      new G4UIcmdWithADoubleAndUnit("/Xenon/geometry/SetGasPressure", this);

    setGasPressCmd->SetGuidance("Set gas pressure.");
    setGasPressCmd->SetUnitCategory("Pressure");
    setGasPressCmd->SetDefaultValue(0.3 * bar);
    setGasPressCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger() {
    delete miniDir;
    delete geometryDir;
    delete setGasPressCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValues) {
  if (command == setGasPressCmd)
    detector->SetGasPressure(setGasPressCmd->GetNewDoubleValue(newValues));
}
