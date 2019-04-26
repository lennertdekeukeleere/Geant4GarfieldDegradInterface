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

  GasModelParameters* fGasModelParameters;
  G4UIdirectory* GasModelParametersDir;
  G4UIdirectory* DegradDir;

  G4UIcmdWithADoubleAndUnit* thermalEnergyCmd;
  
};

#endif
