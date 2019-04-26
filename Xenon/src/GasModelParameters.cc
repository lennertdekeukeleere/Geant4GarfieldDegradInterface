#include "GasModelParameters.hh"
#include "DegradModel.hh"
#include "GasModelParametersMessenger.hh"
#include "DetectorConstruction.hh"

GasModelParameters::GasModelParameters(){
	fMessenger = new GasModelParametersMessenger(this);
}
