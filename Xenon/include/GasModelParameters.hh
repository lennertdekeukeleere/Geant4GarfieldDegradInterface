#ifndef GasModelParameters_hh
#define GasModelParameters_hh

#include "G4SystemOfUnits.hh"
#include "G4String.hh"
#include <map>

class DegradModel;
class GasModelParametersMessenger;
class DetectorConstruction;
class G4String;

class GasModelParameters{
	public:
	
	GasModelParameters();
	~GasModelParameters();
    
    /*Getters and Setters*/
    inline void SetThermalEnergy(G4double d){thermalE=d;}
    inline G4double GetThermalEnergy(){return thermalE;};

	
	private:
	GasModelParametersMessenger* fMessenger;
    G4double thermalE;

};

#endif
