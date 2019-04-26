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
/*!/ALICE/geometry/SetGasPressure*/
/*!/ALICE/geometry/SetNrUpperPlanes*/
/*!/ALICE/geometry/SetMaxStep*/
/*!/ALICE/geometry/SetBField*/
/*!/ALICE/geometry/EMField_version */
/*!/ALICE/geometry/ConstructWires */
/*!/ALICE/geometry/ConstructSlits5Vertical */
/*!/ALICE/geometry/ConstructSlits3Vertical */
/*!/ALICE/geometry/ConstructSlitVertical */
/*!/ALICE/geometry/ConstructSlitHorizontal */
/*!/ALICE/geometry/buildCells*/
/*!/ALICE/geometry/BuildUpperScint*/
/*!/ALICE/geometry/BuildLowerScint*/
/*!/ALICE/geometry/update */

class DetectorMessenger : public G4UImessenger {
 public:
  DetectorMessenger(DetectorConstruction*);
  ~DetectorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private:
  DetectorConstruction* detector;

  G4UIdirectory* miniDir;      ///<\brief /ALICE/
  G4UIdirectory* geometryDir;  ///<\brief /ALICE/geometry/

  G4UIcmdWithADoubleAndUnit* setGasPressCmd;
  G4UIcmdWithAString* setupNameCmd;
    
    
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
