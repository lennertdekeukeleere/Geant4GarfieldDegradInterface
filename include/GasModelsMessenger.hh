#ifndef GasModelsMessenger_h
#define GasModelsMessenger_h 1

#include "G4SystemOfUnits.hh"
#include "G4UImessenger.hh"

class G4UIcommand;
class GasModels;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*! \class GasModelsMessenger*/
/*! class derived from G4UImessenger*/
/*! List of available commands*/

class GasModelsMessenger : public G4UImessenger {
 public:
  GasModelsMessenger(GasModels*);
  ~GasModelsMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private:

  void AddParticleDegradCommand(G4String newValues);
  void AddParticleHeedInterfaceCommand(G4String newValues);
  void AddParticleHeedOnlyCommand(G4String newValues);
  void ConvertParameters(G4String newValues);

  GasModels* fGasModels;
  G4UIdirectory GasModelsDir;
  G4UIdirectory* HeedDir;
  G4UIdirectory* DegradDir;
  G4UIdirectory* HeedOnlyDir;
  G4UIdirectory* HeedInterfaceDir;

  G4UIcommand* addParticleDegradCmd;
  G4UIcommand* addParticleHeedInterfaceCmd;
  G4UIcommand* addParticleHeedOnlyCmd;
  G4UIcmdWithAString* gasFileCmd;
  G4UIcmdWithAString* ionMobFileCmd;
  G4UIcmdWithABool* driftElectronsCmd;
  G4UIcmdWithABool* trackMicroCmd;
  G4UIcmdWithABool* createAvalCmd;
  G4UIcmdWithABool* visualizeChamberCmd;
  G4UIcmdWithABool* visualizeSignalsCmd;
  G4UIcmdWithABool* visualizeFieldCmd;
  G4UIcmdWithABool* driftRKFCmd;
  G4UIcmdWithADouble* voltagePlaneHVCmd;
  G4UIcmdWithADouble* voltagePlaneLowCmd;
  G4UIcmdWithADouble* voltageAnodeWiresCmd;
  G4UIcmdWithADouble* voltageCathodeWiresCmd;
  G4UIcmdWithADouble* voltageGateCmd;
  G4UIcmdWithADouble* voltageDeltaGateCmd;

  G4String fParticleName;
  G4double fEmin;
  G4double fEmax,
  
};