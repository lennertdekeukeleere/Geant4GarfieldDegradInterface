#include "GasModelsMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"


#include "G4Tokenizer.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GasModelsMessenger::GasModelsMessenger(GasModels* gm)
    : fGasModels(gm) {
  GasModelsDir = new G4UIdirectory("/gasmodels/");
  GasModelsDir->SetGuidance("GasModels specific controls");
  HeedDir = new G4UIdirectory("/gasmodels/heed/");
  HeedDir->SetGuidance("Heed specific controls");
  HeedOnlyDir = new G4UIdirectory("/gasmodels/heed/heedonly/");
  HeedOnlyDir->SetGuidance("HeedOnly specific controls");
  HeedInterfaceDir = new G4UIdirectory("/gasmodels/heed/heedinterface/");
  HeedInterfaceDir->SetGuidance("HeedInterface specific controls");
  DegradDir = new G4UIdirectory("/gasmodels/degrad/");
  HeedDir->SetGuidance("Degrad specific controls");

  addParticleDegradCmd = new G4UIcommand("/gasmodels/degrad/addparticle",this);
  addParticleDegradCmd->SetGuidance("Set properties of the particle to be included");
  addParticleDegradCmd->SetGuidance("[usage] /gasModels/degrad/addparticle P Emin Emax");
  addParticleDegradCmd->SetGuidance("        P:(String) particle name (e-, e+, p, mu+, mu-, mu, pi,...");
  addParticleDegradCmd->SetGuidance("        Emin:(double) Minimum energy for the model to be activated");
  addParticleDegradCmd->SetGuidance("        Emax:(double Maximum energy for the model to be activated");

  G4UIparameter* paramD;
  paramD = new G4UIparameter("P");
  paramD->SetDefaultValue("e-");
  addParticleDegradCmd->SetParameter(paramD);
  paramD = new G4UIparameter("Emin",'d',true);
  paramD->SetDefaultValue("0.001");
  addParticleDegradCmd->SetParameter(paramD);
  paramD = new G4UIparameter("Emax",'d',true);
  paramD->SetDefaultValue("1000.");
  addParticleDegradCmd->SetParameter(paramD);

  addParticleHeedInterfaceCmd = new G4UIcommand("/gasmodels/heed/heedinterface/addparticle",this);
  addParticleHeedInterfaceCmd->SetGuidance("Set properties of the particle to be included");
  addParticleHeedInterfaceCmd->SetGuidance("[usage] /gasmodels/heedinterface/addparticle P Emin Emax");
  addParticleHeedInterfaceCmd->SetGuidance("        P:(String) particle name (e-, e+, p, mu+, mu-, mu, pi,...");
  addParticleHeedInterfaceCmd->SetGuidance("        Emin:(double) Minimum energy for the model to be activated");
  addParticleHeedInterfaceCmd->SetGuidance("        Emax:(double Maximum energy for the model to be activated");

  G4UIparameter* paramHI;
  paramHI = new G4UIparameter("P");
  paramHI->SetDefaultValue("e-");
  addParticleHeedInterfaceCmd->SetParameter(paramHI);
  paramHI = new G4UIparameter("Emin",'d',true);
  paramHI->SetDefaultValue("0.001");
  addParticleHeedInterfaceCmd->SetParameter(paramHI);
  paramHI = new G4UIparameter("Emax",'d',true);
  paramHI->SetDefaultValue("1000.");
  addParticleHeedInterfaceCmd->SetParameter(paramHI);

  addParticleHeedOnlyCmd = new G4UIcommand("/gasmodels/heed/heedonly/addparticle",this);
  addParticleHeedOnlyCmd->SetGuidance("Set properties of the particle to be included");
  addParticleHeedOnlyCmd->SetGuidance("[usage] /gasmodels/heed/heedonly/addparticle P Emin Emax");
  addParticleHeedOnlyCmd->SetGuidance("        P:(String) particle name (e-, e+, p, mu+, mu-, mu, pi,...");
  addParticleHeedOnlyCmd->SetGuidance("        Emin:(double) Minimum energy for the model to be activated");
  addParticleHeedOnlyCmd->SetGuidance("        Emax:(double Maximum energy for the model to be activated");

  G4UIparameter* paramHO;
  paramHO = new G4UIparameter("P");
  paramHO->SetDefaultValue("e-");
  addParticleHeedOnlyCmd->SetParameter(paramHO);
  paramHO = new G4UIparameter("Emin",'d',true);
  paramHO->SetDefaultValue("0.001");
  addParticleHeedOnlyCmd->SetParameter(paramHO);
  paramHO = new G4UIparameter("Emax",'d',true);
  paramHO->SetDefaultValue("1000.");
  addParticleHeedOnlyCmd->SetParameter(paramHO);

  gasFileCmd =  new G4UIcmdWithAString("/gasmodels/heed/gasfile",this);
  gasFileCmd->SetGuidance("Set name of the gas file");

  ionMobFileCmd =  new G4UIcmdWithAString("/gasmodels/heed/ionmobilityfile",this);
  ionMobFileCmd->SetGuidance("Set name of the ion mobility file");

  driftElectronsCmd = new G4UIcmdWithABool("/gasmodels/heed/drift");
  driftElectronsCmd->SetGuidance("true if ions and electrons are to be drifted in the electric field");

  driftRKFCmd = new G4UIcmdWithABool("gasmodels/heed/drift");
  driftRKFCmd->SetGuidance("true if runge kutta is used for the drift");

  createAvalCmd = new G4UIcmdWithABool("/gasmodels/heed/trackmicroscopic");
  createAvalCmd->SetGuidance("true if monte carlo simulation of an avalanches is to be used");

  trackMicroCmd = new G4UIcmdWithABool("/gasmodels/heed/trackmicroscopic");
  trackMicroCmd->SetGuidance("true if microscopic tracking of the drift electrons/ions and avalanche is to be used");

  visualizeChamberCmd = new G4UIcmdWithABool("/gasmodels/heed/visualizechamber");
  visualizeChamberCmd->SetGuidance("true if visualization of the chamber configuration has to be shown");

  visualizeSignalsCmd = new G4UIcmdWithABool("/gasmodels/heed/visualizesignals");
  visualizeSignalsCmd->SetGuidance("true if signal on the pads have to be visualized");  

  visualizeFieldCmd = new G4UIcmdWithABool("/gasmodels/heed/visualizefield");
  visualizeFieldCmd->SetGuidance("true if the electric field has to be shown");

  voltagePlaneHVCmd = new G4UIcmdWithADouble("/gasmodels/heed/voltageplanehv");
  voltagePlaneHVCmd->SetGuidance("Set the voltage on the high voltage plane");

  voltagePlaneLowCmd = new G4UIcmdWithADouble("/gasmodels/heed/voltageplanelow");
  voltagePlaneLowCmd->SetGuidance("Set the voltage on the low voltage plane");

  voltageAnodeWiresCmd = new G4UIcmdWithADouble("/gasmodels/heed/voltageanodewire");
  voltageAnodeWiresCmd->SetGuidance("Set the voltage on the anode wire");

  voltageCathodeWiresCmd = new G4UIcmdWithADouble("/gasmodels/heed/voltagecathodewire");
  voltageCathodeWiresCmd->SetGuidance("Set the voltage on the cathode wire");

  voltageGateCmd = new G4UIcmdWithADouble("/gasmodels/heed/voltagegate");
  voltageGateCmd->SetGuidance("Set the voltage of the gate centroid value");

  voltageDeltaGateCmd = new G4UIcmdWithADouble("/gasmodels/heed/voltagedeltagate");
  voltageDeltaGateCmd->SetGuidance("Set the voltage difference of the gate wires with respect to the centroid: v + dv, v-dv");

  
  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GasModelsMessenger::~GasModelsMessenger() {
  delete GasModelsDir;
  delete HeedDir;
  delete DegradDir;
  delete HeedOnlyDir;
  delete HeedInterfaceDir;

  delete addParticleDegradCmd;
  delete addParticleHeedInterfaceCmd;
  delete addParticleHeedOnlyCmd;
  delete gasFileCmd;
  delete ionMobFileCmd;
  delete driftElectronsCmd;
  delete driftRKFCmd;
  delete createAvalCmd;
  delete trackMicroCmd;
  delete visualizeChamberCmd;
  delete visualizeSignalsCmd;
  delete visualizeFieldCmd;
  delete voltagePlaneHVCmd;
  delete voltagePlaneLowCmd;
  delete voltageAnodeWiresCmd;
  delete voltageCathodeWiresCmd;
  delete voltageGateCmd;
  delete voltageDeltaGateCmd;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GasModelsMessenger::SetNewValue(G4UIcommand* command, G4String newValues) {
	  if(command == addParticleDegradCmd)
	  	AddParticleDegradCommand(newValues);
	  else if(command == addParticleHeedInterfaceCmd)
	  	AddParticleHeedInterfaceCommand(newValues);
	  else if(command == addParticleHeedOnlyCmd)
	  	AddParticleHeedOnlyCommand(newValues);
	  else if(command == gasFileCmd){
	  	(fGasModels->GetHeedInterfaceModel())->SetGasFile(newValues);
	  	(fGasModels->GetHeedOnlyModel())->SetGasFile(newValues);
	  }
	  else if(command == ionMobFileCmd){
	  	(fGasModels->GetHeedInterfaceModel())->SetIonMobilityFile(newValues);
	  	(fGasModels->GetHeedOnlyModel())->SetIonMobilityFile(newValues);
	  }
	  else if(command == driftElectronsCmd){
	  	(fGasModels->GetHeedInterfaceModel())->SetDriftElectrons(driftElectronsCmd->GetNewBoolValue(newValues));
	  	(fGasModels->GetHeedOnlyModel())->SetDriftElectrons(driftElectronsCmd->GetNewBoolValue(newValues));
	  }
	  else if(command == driftRKFCmd){
	  	(fGasModels->GetHeedInterfaceModel())->SetDriftRKF(driftRKFCmd->GetNewBoolValue(newValues));
	  	(fGasModels->GetHeedOnlyModel())->SetDriftRKF(driftRKFCmd->GetNewBoolValue(newValues));
	  }
	  else if(command == createAvalCmd){
	  	(fGasModels->GetHeedInterfaceModel())->SetCreateAvalancheMC(createAvalCmd->GetNewBoolValue(newValues));
	  	(fGasModels->GetHeedOnlyModel())->SetCreateAvalancheMC(createAvalCmd->GetNewBoolValue(newValues));
	  }
	  else if(command == trackMicroCmd){
	  	(fGasModels->GetHeedInterfaceModel())->SetTrackMicroscopic(trackMicroCmd->GetNewBoolValue(newValues));
	  	(fGasModels->GetHeedOnlyModel())->SetTrackMicroscopic(trackMicroCmd->GetNewBoolValue(newValues));
	  }
	  else if(command == visualizeChamberCmd){
	  	(fGasModels->GetHeedInterfaceModel())->SetVisualizeChamber(visualizeChamberCmd->GetNewBoolValue(newValues));
	  	(fGasModels->GetHeedOnlyModel())->SetVisualizeChamber(visualizeChamberCmd->GetNewBoolValue(newValues));
	  }
	  else if(command == visualizeSignalsCmd){
	  	(fGasModels->GetHeedInterfaceModel())->SetVisualizeSignals(visualizeSignalsCmd->GetNewBoolValue(newValues));
	  	(fGasModels->GetHeedOnlyModel())->SetVisualizeSignals(visualizeSignalsCmd->GetNewBoolValue(newValues));
	  }
	  else if(command == visualizeFieldCmd){
	  	(fGasModels->GetHeedInterfaceModel())->SetVisualizeField(visualizeFieldCmd->GetNewBoolValue(newValues));
	  	(fGasModels->GetHeedOnlyModel())->SetVisualizeField(visualizeFieldCmd->GetNewBoolValue(newValues));
	  }
	  else if(command == voltagePlaneHVCmd){
	  	(fGasModels->GetHeedInterfaceModel())->SetVoltagePlaneHV(voltagePlaneHVCmd->GetNewDoubleValue(newValues));
	  	(fGasModels->GetHeedOnlyModel())->SetVoltagePlaneHV(voltagePlaneHVCmd->GetNewDoubleValue(newValues));
	  }
	  else if(command == voltagePlaneLowCmd){
	  	(fGasModels->GetHeedInterfaceModel())->SetVoltagePlaneLow(voltagePlaneLowCmd->GetNewDoubleValue(newValues));
	  	(fGasModels->GetHeedOnlyModel())->SetVoltagePlaneLow(voltagePlaneLowCmd->GetNewDoubleValue(newValues));
	  }
	  else if(command == voltageAnodeWiresCmd){
	  	(fGasModels->GetHeedInterfaceModel())->SetVoltageAnodeWires(voltageAnodeWiresCmd->GetNewDoubleValue(newValues));
	  	(fGasModels->GetHeedOnlyModel())->SetVoltageAnodeWires(voltageAnodeWiresCmd->GetNewDoubleValue(newValues));
	  }
	  else if(command == voltageCathodeWiresCmd){
	  	(fGasModels->GetHeedInterfaceModel())->SetVoltageCathodeWires(voltageCathodeWiresCmd->GetNewDoubleValue(newValues));
	  	(fGasModels->GetHeedOnlyModel())->SetVoltageCathodeWires(voltageCathodeWiresCmd->GetNewDoubleValue(newValues));
	  }
	  else if(command == voltageGateCmd){
	  	(fGasModels->GetHeedInterfaceModel())->SetVoltageGate(voltageGateCmd->GetNewDoubleValue(newValues));
	  	(fGasModels->GetHeedOnlyModel())->SetVoltageGate(voltageGateCmd->GetNewDoubleValue(newValues));
	  }
	  else if(command == voltageDeltaGateCmd){
	  	(fGasModels->GetHeedInterfaceModel())->SetVoltageDeltaGate(voltageDeltaGateCmd->GetNewDoubleValue(newValues));
	  	(fGasModels->GetHeedOnlyModel())->SetVoltageDeltaGate(voltageDeltaGateCmd->GetNewDoubleValue(newValues));
	  }

}

void GasModelsMessenger::AddParticleDegradCommand(G4String newValues){
	ConvertParameters();
	(fGasModels->GetDegradModel())->AddParticleName(fParticleName,fEmin/keV,fEmax/keV);
}

void GasModelsMessenger::AddParticleHeedInterfaceCommand(G4String newValues){
	ConvertParameters();
	(fGasModels->GetHeedInterfaceModel())->AddParticleName(fParticleName,fEmin/keV,fEmax/keV);
}

void GasModelsMessenger::AddParticleHeedOnlyCommand(G4String newValues){
	ConvertParameters();
	(fGasModels->GetHeedOnlyModel())->AddParticleName(fParticleName,fEmin/keV,fEmax/keV);
}

void GasModels::ConvertParameters(G4String newValues){
	G4Tokenizer next( newValues );
	fParticleName = next();
	G4String Semin = next();
	if(Semin.isNull()){
	 	fEmin = 1. *keV;
	 	fEmax = 1. *GeV;
	}
	else{
		fEmin = StoD(Semin);
		G4String Semax = next();
		if(Semax.isNull())
		 	fEmax = 1.*GeV;
		else
			fEmax = StoD(Semax);
	}
}