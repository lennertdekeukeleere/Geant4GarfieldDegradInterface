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
#include "GasBoxSD.hh"

#include "G4AutoLock.hh"
namespace{G4Mutex aMutex = G4MUTEX_INITIALIZER;}


HeedInterfaceModel::HeedInterfaceModel(GasModelParameters* gmp,G4String modelName, G4Region* envelope,DetectorConstruction* dc, GasBoxSD* sd)
    : HeedModel(modelName, envelope,dc) {
        *fMapParticlesEnergy = gmp->GetParticleNamesHeedInterface();
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
        fGasBoxHitsCollection = sd->GetGasBoxHitsCollection();
    }

HeedInterfaceModel::~HeedInterfaceModel() {}

void HeedInterfaceModel::Run(G4String particleName, double ekin_keV, double t, double x_cm,
            double y_cm, double z_cm, double dx, double dy, double dz){
    double eKin_eV = ekin_keV * 1000;
    int nc = 0, ni=0;
    if(particleName == "e-"){
        G4AutoLock lock(&aMutex);
        fTrackHeed->TransportDeltaElectron(x_cm, y_cm, z_cm, t, eKin_eV, dx, dy,
                                           dz, nc, ni);
    }
    else{
        G4AutoLock lock(&aMutex);
        fTrackHeed->TransportPhoton(x_cm, y_cm, z_cm, t, eKin_eV, dx, dy,
                                    dz, nc);
    }                        
}

void HeedInterfaceModel::ProcessEvent(){

}

void HeedInterfaceModel::Reset(){
  
}


