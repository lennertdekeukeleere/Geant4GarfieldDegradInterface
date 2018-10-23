#ifndef HeedMessenger_h
#define HeedMessenger_h 1

#include "G4SystemOfUnits.hh"
#include "G4UImessenger.hh"

class G4UIcommand;
class HeedInterfaceModel;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*! \class HeedMessenger*/
/*! class derived from G4UImessenger*/
/*! List of available commands*/

class HeedMessenger : public G4UImessenger {
 public:
  HeedMessenger(HeedInterfaceModel*);
  ~HeedMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private:
  HeedModel* fHeedModel;

  G4UIdirectory* HeedDir;
  G4UIcmdWithAString* gasFileCmd;
  G4UIcmdWithAString* ionMobFileCmd;
  G4UIcmdWithoutParameter* initCmd;
  G4UIcmdWithABool* driftElectronsCmd;
  G4UIcmdWithADoubleAndUnit* ELimitCmd;
  G4UIcmdWithABool* trackMicroCmd;
  G4UIcmdWithABool* createAvalCmd;
  G4UIcmdWithABool* visualizeChamberCmd;
  G4UIcmdWithABool* visualizeSignalsCmd;
  G4UIcmdWithABool* visualizeFieldCmd;
  G4UIcmdWithABool* driftRKFCmd;
  G4UIcmdWithADouble* voltagePlaneOneCmd;
  G4UIcmdWithADouble* voltagePlaneTwoCmd;
  G4UIcmdWithADouble* voltageAnodeWiresCmd;
  G4UIcmdWithADouble* voltageCathodeWiresCmd;
  G4UIcmdWithADouble* voltageGateCmd;
  G4UIcmdWithADouble* voltageDeltaGateCmd;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
