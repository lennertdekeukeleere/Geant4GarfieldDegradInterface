#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "G4SystemOfUnits.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithoutParameter;
class G4UIcmdWith3Vector;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*! \class DetectorMessenger*/
/*! class derived from G4UImessenger*/
/*! List of available commands*/
/*!/InterfaceExample/geometry/SetGasPressure*/
/*!/InterfaceExample/geometry/SetNrUpperPlanes*/
/*!/InterfaceExample/geometry/SetMaxStep*/
/*!/InterfaceExample/geometry/SetBField*/
/*!/InterfaceExample/geometry/EMField_version */
/*!/InterfaceExample/geometry/ConstructWires */
/*!/InterfaceExample/geometry/ConstructSlits5Vertical */
/*!/InterfaceExample/geometry/ConstructSlits3Vertical */
/*!/InterfaceExample/geometry/ConstructSlitVertical */
/*!/InterfaceExample/geometry/ConstructSlitHorizontal */
/*!/InterfaceExample/geometry/buildCells*/
/*!/InterfaceExample/geometry/BuildUpperScint*/
/*!/InterfaceExample/geometry/BuildLowerScint*/
/*!/InterfaceExample/geometry/update */

class DetectorMessenger : public G4UImessenger {
 public:
  DetectorMessenger(DetectorConstruction*);
  ~DetectorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private:
  DetectorConstruction* detector;

  G4UIdirectory* miniDir;      ///<\brief /InterfaceExample/
  G4UIdirectory* geometryDir;  ///<\brief /InterfaceExample/geometry/

  G4UIcmdWithADoubleAndUnit* setGasPressCmd;
  G4UIcmdWithADouble* setAddmixturePercCmd;
  G4UIcmdWithAString* gasNameCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
