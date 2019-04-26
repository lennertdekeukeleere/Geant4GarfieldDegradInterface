#ifndef GasModelParametersMessenger_h
#define GasModelParametersMessenger_h 1

#include "G4SystemOfUnits.hh"
#include "G4UImessenger.hh"

class G4UIcommand;
class GasModelParameters;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*! \class GasModelParametersMessenger*/
/*! class derived from G4UImessenger*/
/*! List of available commands*/

class GasModelParametersMessenger : public G4UImessenger {
 public:
  GasModelParametersMessenger(GasModelParameters*);
  ~GasModelParametersMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private:

  void AddParticleHeedDeltaElectronCommand(G4String newValues);
  void AddParticleHeedNewTrackCommand(G4String newValues);
  void ConvertParameters(G4String newValues);

  GasModelParameters* fGasModelParameters;
  G4UIdirectory* GasModelParametersDir;
  G4UIdirectory* HeedDir;
  G4UIdirectory* HeedNewTrackDir;
  G4UIdirectory* HeedDeltaElectronDir;

  G4UIcommand* addParticleDegradCmd;
  G4UIcommand* addParticleHeedDeltaElectronCmd;
  G4UIcommand* addParticleHeedNewTrackCmd;
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
  G4double fEmax;
  
};

#endif
