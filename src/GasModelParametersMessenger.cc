#include "GasModelParametersMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4UIparameter.hh"
#include "GasModelParameters.hh"
#include "HeedInterfaceModel.hh"
#include "HeedOnlyModel.hh"
#include "HeedModel.hh"
#include "DegradModel.hh"

#include "G4Tokenizer.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GasModelParametersMessenger::GasModelParametersMessenger(GasModelParameters* gm)
    : fGasModelParameters(gm) {
  GasModelParametersDir = new G4UIdirectory("/gasModelParameters/");
  GasModelParametersDir->SetGuidance("GasModelParameters specific controls");
  HeedDir = new G4UIdirectory("/gasModelParameters/heed/");
  HeedDir->SetGuidance("Heed specific controls");
  HeedOnlyDir = new G4UIdirectory("/gasModelParameters/heed/heedonly/");
  HeedOnlyDir->SetGuidance("HeedOnly specific controls");
  HeedInterfaceDir = new G4UIdirectory("/gasModelParameters/heed/heedinterface/");
  HeedInterfaceDir->SetGuidance("HeedInterface specific controls");
  DegradDir = new G4UIdirectory("/gasModelParameters/degrad/");
  HeedDir->SetGuidance("Degrad specific controls");

  addParticleHeedInterfaceCmd = new G4UIcommand("/gasModelParameters/heed/heedinterface/addparticle",this);
  addParticleHeedInterfaceCmd->SetGuidance("Set properties of the particle to be included");
  addParticleHeedInterfaceCmd->SetGuidance("[usage] /gasModelParameters/heedinterface/addparticle P Emin Emax");
  addParticleHeedInterfaceCmd->SetGuidance("        P:(String) particle name (e-, e+, p, mu+, mu-, mu, pi,...");
  addParticleHeedInterfaceCmd->SetGuidance("        Emin:(double) Minimum energy for the model to be activated");
  addParticleHeedInterfaceCmd->SetGuidance("        Emax:(double Maximum energy for the model to be activated");

  G4UIparameter* paramHI;
  paramHI = new G4UIparameter("P",'s',false);
  paramHI->SetDefaultValue("e-");
  addParticleHeedInterfaceCmd->SetParameter(paramHI);
  paramHI = new G4UIparameter("Emin",'d',true);
  paramHI->SetDefaultValue("0.001");
  addParticleHeedInterfaceCmd->SetParameter(paramHI);
  paramHI = new G4UIparameter("Emax",'d',true);
  paramHI->SetDefaultValue("1000.");
  addParticleHeedInterfaceCmd->SetParameter(paramHI);

  addParticleHeedOnlyCmd = new G4UIcommand("/gasModelParameters/heed/heedonly/addparticle",this);
  addParticleHeedOnlyCmd->SetGuidance("Set properties of the particle to be included");
  addParticleHeedOnlyCmd->SetGuidance("[usage] /gasModelParameters/heed/heedonly/addparticle P Emin Emax");
  addParticleHeedOnlyCmd->SetGuidance("        P:(String) particle name (e-, e+, p, mu+, mu-, mu, pi,...");
  addParticleHeedOnlyCmd->SetGuidance("        Emin:(double) Minimum energy for the model to be activated");
  addParticleHeedOnlyCmd->SetGuidance("        Emax:(double Maximum energy for the model to be activated");

  G4UIparameter* paramHO;
  paramHO = new G4UIparameter("P",'s',false);
  paramHO->SetDefaultValue("e-");
  addParticleHeedOnlyCmd->SetParameter(paramHO);
  paramHO = new G4UIparameter("Emin",'d',true);
  paramHO->SetDefaultValue("0.001");
  addParticleHeedOnlyCmd->SetParameter(paramHO);
  paramHO = new G4UIparameter("Emax",'d',true);
  paramHO->SetDefaultValue("1000.");
  addParticleHeedOnlyCmd->SetParameter(paramHO);

  gasFileCmd =  new G4UIcmdWithAString("/gasModelParameters/heed/gasfile",this);
  gasFileCmd->SetGuidance("Set name of the gas file");

  ionMobFileCmd =  new G4UIcmdWithAString("/gasModelParameters/heed/ionmobilityfile",this);
  ionMobFileCmd->SetGuidance("Set name of the ion mobility file");

  driftElectronsCmd = new G4UIcmdWithABool("/gasModelParameters/heed/drift",this);
  driftElectronsCmd->SetGuidance("true if ions and electrons are to be drifted in the electric field");

  driftRKFCmd = new G4UIcmdWithABool("/gasModelParameters/heed/driftRKF",this);
  driftRKFCmd->SetGuidance("true if runge kutta is used for the drift");

  createAvalCmd = new G4UIcmdWithABool("/gasModelParameters/heed/createAval",this);
  createAvalCmd->SetGuidance("true if monte carlo simulation of an avalanches is to be used");

  trackMicroCmd = new G4UIcmdWithABool("/gasModelParameters/heed/trackmicroscopic",this);
  trackMicroCmd->SetGuidance("true if microscopic tracking of the drift electrons/ions and avalanche is to be used");

  visualizeChamberCmd = new G4UIcmdWithABool("/gasModelParameters/heed/visualizechamber",this);
  visualizeChamberCmd->SetGuidance("true if visualization of the chamber configuration has to be shown");

  visualizeSignalsCmd = new G4UIcmdWithABool("/gasModelParameters/heed/visualizesignals",this);
  visualizeSignalsCmd->SetGuidance("true if signal on the pads have to be visualized");  

  visualizeFieldCmd = new G4UIcmdWithABool("/gasModelParameters/heed/visualizefield",this);
  visualizeFieldCmd->SetGuidance("true if the electric field has to be shown");

  voltagePlaneHVCmd = new G4UIcmdWithADouble("/gasModelParameters/heed/voltageplanehv",this);
  voltagePlaneHVCmd->SetGuidance("Set the voltage on the high voltage plane");

  voltagePlaneLowCmd = new G4UIcmdWithADouble("/gasModelParameters/heed/voltageplanelow",this);
  voltagePlaneLowCmd->SetGuidance("Set the voltage on the low voltage plane");

  voltageAnodeWiresCmd = new G4UIcmdWithADouble("/gasModelParameters/heed/voltageanodewire",this);
  voltageAnodeWiresCmd->SetGuidance("Set the voltage on the anode wire");

  voltageCathodeWiresCmd = new G4UIcmdWithADouble("/gasModelParameters/heed/voltagecathodewire",this);
  voltageCathodeWiresCmd->SetGuidance("Set the voltage on the cathode wire");

  voltageGateCmd = new G4UIcmdWithADouble("/gasModelParameters/heed/voltagegate",this);
  voltageGateCmd->SetGuidance("Set the voltage of the gate centroid value");

  voltageDeltaGateCmd = new G4UIcmdWithADouble("/gasModelParameters/heed/voltagedeltagate",this);
  voltageDeltaGateCmd->SetGuidance("Set the voltage difference of the gate wires with respect to the centroid: v + dv, v-dv");

  thermalEnergyCmd = new G4UIcmdWithADoubleAndUnit("/gasModelParameters/degrad/thermalenergy",this);
  thermalEnergyCmd->SetGuidance("Set the thermal energy to be used by degrad");
  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GasModelParametersMessenger::~GasModelParametersMessenger() {
  delete GasModelParametersDir;
  delete HeedDir;
  delete DegradDir;
  delete HeedOnlyDir;
  delete HeedInterfaceDir;

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
  delete thermalEnergyCmd;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GasModelParametersMessenger::SetNewValue(G4UIcommand* command, G4String newValues) {
	  if(command == addParticleHeedInterfaceCmd)
	  	AddParticleHeedInterfaceCommand(newValues);
	  else if(command == addParticleHeedOnlyCmd)
	  	AddParticleHeedOnlyCommand(newValues);
	  else if(command == gasFileCmd){
	  	fGasModelParameters->SetGasFile(newValues);
	  }
	  else if(command == ionMobFileCmd){
	  	fGasModelParameters->SetIonMobilityFile(newValues);
	  }
	  else if(command == driftElectronsCmd){
	  	fGasModelParameters->SetDriftElectrons(driftElectronsCmd->GetNewBoolValue(newValues));
	  }
	  else if(command == driftRKFCmd){
	  	fGasModelParameters->SetDriftRKF(driftRKFCmd->GetNewBoolValue(newValues));
	  }
	  else if(command == createAvalCmd){
	  	fGasModelParameters->SetCreateAvalancheMC(createAvalCmd->GetNewBoolValue(newValues));
	  }
	  else if(command == trackMicroCmd){
	  	fGasModelParameters->SetTrackMicroscopic(trackMicroCmd->GetNewBoolValue(newValues));
	  }
	  else if(command == visualizeChamberCmd){
	  	fGasModelParameters->SetVisualizeChamber(visualizeChamberCmd->GetNewBoolValue(newValues));
	  }
	  else if(command == visualizeSignalsCmd){
	  	fGasModelParameters->SetVisualizeSignals(visualizeSignalsCmd->GetNewBoolValue(newValues));
	  }
	  else if(command == visualizeFieldCmd){
	  	fGasModelParameters->SetVisualizeField(visualizeFieldCmd->GetNewBoolValue(newValues));
	  }
	  else if(command == voltagePlaneHVCmd){
	  	fGasModelParameters->SetVoltagePlaneHV(voltagePlaneHVCmd->GetNewDoubleValue(newValues));
	  }
	  else if(command == voltagePlaneLowCmd){
	  	fGasModelParameters->SetVoltagePlaneLow(voltagePlaneLowCmd->GetNewDoubleValue(newValues));
	  }
	  else if(command == voltageAnodeWiresCmd){
	  	fGasModelParameters->SetVoltageAnodeWires(voltageAnodeWiresCmd->GetNewDoubleValue(newValues));
	  }
	  else if(command == voltageCathodeWiresCmd){
	  	fGasModelParameters->SetVoltageCathodeWires(voltageCathodeWiresCmd->GetNewDoubleValue(newValues));
	  }
	  else if(command == voltageGateCmd){
	  	fGasModelParameters->SetVoltageGate(voltageGateCmd->GetNewDoubleValue(newValues));
      }
	  else if(command == voltageDeltaGateCmd){
	  	fGasModelParameters->SetVoltageDeltaGate(voltageDeltaGateCmd->GetNewDoubleValue(newValues));
	  }
    else if(command == thermalEnergyCmd){
      fGasModelParameters->SetThermalEnergy(thermalEnergyCmd->GetNewDoubleValue(newValues));
    }

}

void GasModelParametersMessenger::AddParticleHeedInterfaceCommand(G4String newValues){
	ConvertParameters(newValues);
	fGasModelParameters->AddParticleNameHeedInterface(fParticleName,fEmin/keV,fEmax/keV);
}

void GasModelParametersMessenger::AddParticleHeedOnlyCommand(G4String newValues){
	ConvertParameters(newValues);
	fGasModelParameters->AddParticleNameHeedOnly(fParticleName,fEmin/keV,fEmax/keV);
}

void GasModelParametersMessenger::ConvertParameters(G4String newValues){
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
