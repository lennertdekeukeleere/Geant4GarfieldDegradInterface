/*
 * HeedInterfaceModel.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: dpfeiffe
 */
#include <iostream>
#include "HeedInterfaceModel.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "GasModelParameters.hh"


HeedInterfaceModel::HeedInterfaceModel(GasModelParameters* gmp,G4String modelName, G4Region* envelope,DetectorConstruction* dc)
    : HeedModel(modelName, envelope,dc) {
        fMapParticlesEnergy = &(gmp->GetParticleNamesHeedInterface());
        gasFile = gmp->GetGasFile();
        ionMobFile = gmp->GetIonMobilityFile();
        driftElectrons = gmp->GetDriftElectrons();
        trackMicro = gmp->GetTrackMicroscopic();
        createAval = gmp->GetCreateAvalancheMC();
        fVisualizeChamber = gmp->GetVisualizeChamber();
        fVisualizeSignal = gmp->GetVisualizeSignals();
        fVisualizeField = gmp->GetVisualizeField();
        driftRKF = gmp->GetDriftRKF();
        vPlaneHV = gmp->GetVoltagePlaneHV();
        vPlaneLow = gmp->GetVoltagePlaneLow();
        vAnodeWires = gmp->GetVoltageAnodeWires();
        vCathodeWires = gmp->GetVoltageCathodeWires();
        vGate = gmp->GetVoltageGate();
        vDeltaGate = gmp->GetVoltageDeltaGate();
    }

HeedInterfaceModel::~HeedInterfaceModel() {}

void HeedInterfaceModel::Run(G4String particleName, double ekin_keV, double t, double x_cm,
            double y_cm, double z_cm, double dx, double dy, double dz){

}

void HeedInterfaceModel::ProcessEvent(){

}

void HeedInterfaceModel::Reset(){
  
}


