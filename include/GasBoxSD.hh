#ifndef GasBoxSD_hh
#define GasBoxSD_hh

#include "G4VSensitiveDetector.hh"
#include "G4String.hh"
#include "G4Region.hh"
#include "GasBoxHit.hh"
#include "DriftLineHit.hh"
#include "XenonHit.hh"
#include "GarfieldExcitationHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;


class GasBoxSD : public G4VSensitiveDetector{
	public:
	
	GasBoxSD(G4String);
	~GasBoxSD();
	
	virtual void 	Initialize (G4HCofThisEvent *);
	virtual void 	EndOfEvent (G4HCofThisEvent *){};
	virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    XenonHitsCollection* GetXenonHitsCollection(){return fXenonHitsCollection;};
    GarfieldExcitationHitsCollection* GetGarfieldExcitationHitsCollection(){return fGarfieldExcitationHitsCollection;};
    GasBoxHitsCollection* GetGasBoxHitsCollection(){return fGasBoxHitsCollection;};
    DriftLineHitsCollection* GetDriftLineHitsCollection(){return fDriftLineHitsCollection;};
	
	private:
	
    GasBoxHitsCollection* fGasBoxHitsCollection;
    XenonHitsCollection* fXenonHitsCollection;
    GarfieldExcitationHitsCollection* fGarfieldExcitationHitsCollection;
    DriftLineHitsCollection* fDriftLineHitsCollection;
    G4int GBHCID;
    G4int DLHCID;
    G4int XHCID;
    G4int GEHCID;
    
	
};

#endif

