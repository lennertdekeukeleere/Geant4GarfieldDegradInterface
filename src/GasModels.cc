#include "GasModels.hh"
#include "DegradModel.hh"
#include "HeedInterfaceModel.hh"
#include "HeedOnlyModel.hh"
#include "GasModelsMessenger.hh"
#include "DetectorContruction.hh"

GasModels::GasModels(DetectorContruction* dc){
	dm = new DegradModel(dc);
	him = new HeedInterfaceModel(dc);
	hom = new HeedOnlyModel(dc);
	fMessenger = new GasModelsMessenger(this);
	dc->SetGasModels(this);
}