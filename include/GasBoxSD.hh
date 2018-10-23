#ifndef GasBoxSD_hh
#define GasBoxSD_hh

#include "G4VSensitiveDetector.hh"
#include "G4String.hh"
#include "G4Region.hh"
#include "GasBoxHit.hh"

class GarfieldModel;
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;


class GasBoxSD : public G4VSensitiveDetector{
	public:
	
	GasBoxSD(G4String, GarfieldModel*);
	~GasBoxSD();
	
	virtual void 	Initialize (G4HCofThisEvent *);
	virtual void 	EndOfEvent (G4HCofThisEvent *){};
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	G4bool AddSecondary(G4ThreeVector, G4double, G4double, G4int, G4int, G4int);
	
	private:
	
    GarfieldModel* garfieldModel;
    GasBoxHitsCollection* fGasBoxHitsCollection;
    G4int HCID;
	G4double edep, ef;
	std::vector<G4double> secEnergiesFirstGen;
	std::vector<G4double> secEnergiesSecGen;
    
	
};

#endif

