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
/*!/Xenon/geometry/SetGasPressure*/
/*!/Xenon/geometry/SetNrUpperPlanes*/
/*!/Xenon/geometry/SetMaxStep*/
/*!/Xenon/geometry/SetBField*/
/*!/Xenon/geometry/EMField_version */
/*!/Xenon/geometry/ConstructWires */
/*!/Xenon/geometry/ConstructSlits5Vertical */
/*!/Xenon/geometry/ConstructSlits3Vertical */
/*!/Xenon/geometry/ConstructSlitVertical */
/*!/Xenon/geometry/ConstructSlitHorizontal */
/*!/Xenon/geometry/buildCells*/
/*!/Xenon/geometry/BuildUpperScint*/
/*!/Xenon/geometry/BuildLowerScint*/
/*!/Xenon/geometry/update */

class DetectorMessenger : public G4UImessenger {
 public:
  DetectorMessenger(DetectorConstruction*);
  ~DetectorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

 private:
  DetectorConstruction* detector;

  G4UIdirectory* miniDir;      ///<\brief /Xenon/
  G4UIdirectory* geometryDir;  ///<\brief /Xenon/geometry/

  G4UIcmdWithADoubleAndUnit* setGasPressCmd;
    
    
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
