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
    miniDir = new G4UIdirectory("/InterfaceExample/");
    miniDir->SetGuidance("InterfaceExample specific controls");

    ////////////////////
    geometryDir = new G4UIdirectory("/InterfaceExample/geometry/");
    geometryDir->SetGuidance("InterfaceExample geometry specific controls");


    ////////////////////
    setGasPressCmd =
      new G4UIcmdWithADoubleAndUnit("/InterfaceExample/geometry/SetGasPressure", this);

    setGasPressCmd->SetGuidance("Set gas pressure.");
    setGasPressCmd->SetUnitCategory("Pressure");
    setGasPressCmd->SetDefaultValue(0.3 * bar);
    setGasPressCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    //////////////////
    setAddmixturePercCmd = new G4UIcmdWithADouble(
      "/InterfaceExample/geometry/SetAddmixturePercentage", this);
    setAddmixturePercCmd->SetGuidance(
      "Set addmixture percentage of gas mixture to x, and the noble gas part is "
      "set to 1-x");
    setAddmixturePercCmd->SetDefaultValue(1.0);
    setAddmixturePercCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    ////////////////////
    gasNameCmd = new G4UIcmdWithAString("/InterfaceExample/geometry/GasName", this);
    gasNameCmd->SetGuidance("Set the mixture: HeIso or ArCO2");
    gasNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger() {
    delete miniDir;
    delete geometryDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValues) {
  if (command == setGasPressCmd)
    detector->SetGasPressure(setGasPressCmd->GetNewDoubleValue(newValues));
  else if (command == setAddmixturePercCmd)
    detector->SetAddmixturePercentage(setAddmixturePercCmd->GetNewDoubleValue(newValues));
  else if (command == gasNameCmd)
      detector->SetGasName(newValues);
}
