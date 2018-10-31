#ifndef EventAction_hh
#define EventAction_hh 1

#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"
#include <vector>

class G4VPhysicalVolume;
class SteppingAction;
class G4Event;
class GarfieldVUVPhotonModel;


class EventAction : public G4UserEventAction {
 public:
  EventAction();
  ~EventAction();

 public:
  void BeginOfEventAction(const G4Event *);
  void EndOfEventAction(const G4Event *);
  

 private:
	GarfieldVUVPhotonModel* fGarfieldVUVPhotonModel;
	
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
