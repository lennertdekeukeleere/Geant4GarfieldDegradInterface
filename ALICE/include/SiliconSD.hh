#ifndef SiliconSD_hh
#define SiliconSD_hh

#include "G4VSensitiveDetector.hh"
#include "G4String.hh"
#include "G4Region.hh"
#include "SiliconHit.hh"


class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;


class SiliconSD : public G4VSensitiveDetector{
	public:
	
	SiliconSD(G4String);
	~SiliconSD();
	
	virtual void 	Initialize (G4HCofThisEvent *);
	virtual void 	EndOfEvent (G4HCofThisEvent *);
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
	
	private:
	
    SiliconHitsCollection* fSiliconHitsCollection;
    G4int SHCID;
    
	
};

#endif

