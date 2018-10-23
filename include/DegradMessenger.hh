#ifndef DegradMessenger_h
#define DegradMessenger_h 1

#include "G4SystemOfUnits.hh"
#include "G4UImessenger.hh"

class G4UIcommand;
class DegradModel;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADouble;
class G4UIcmdWithAnInteger;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*! \class DegradMessenger*/
/*! class derived from G4UImessenger*/
/*! List of available commands*/

class DegradMessenger : public G4UImessenger {
 public:
  DegradMessenger(DegradModel*);
  ~DegradMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private:
  DegradModel* fDegradModel;
  G4UIdirectory* DegradDir;
  
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
