/*
 * HeedNewTrackModel.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: dpfeiffe
 */
#include <iostream>
#include "HeedNewTrackModel.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "GasModelParameters.hh"
#include "GasBoxSD.hh"
#include "GasBoxHit.hh"
#include "G4SDManager.hh"

#include "G4AutoLock.hh"
namespace{G4Mutex aMutex = G4MUTEX_INITIALIZER;}

// HeedNewTrackModel derives from the HeedModel Class and uses the GasModelParameters Class to set some user-defined veriables
HeedNewTrackModel::HeedNewTrackModel(GasModelParameters* gmp,G4String modelName, G4Region* envelope,DetectorConstruction* dc, GasBoxSD* sd)
    : HeedModel(modelName, envelope,dc,sd)	{
        G4cout << "Copying the particle map" << G4endl;
        G4cout << gmp->GetParticleNamesHeedNewTrack().size() << G4endl;
        fMapParticlesEnergy = gmp->GetParticleNamesHeedNewTrack();
        G4cout << "set the gas file" << G4endl;
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
        name="HeedNewTrackModel";
        InitialisePhysics();
    }

HeedNewTrackModel::~HeedNewTrackModel() {}

//This method is called in the DoIt-method in parent class HeedModel
void HeedNewTrackModel::Run(G4String particleName, double ekin_keV, double t, double x_cm,
            double y_cm, double z_cm, double dx, double dy, double dz){
    double eKin_eV = ekin_keV * 1000;
    int nc = 0, ni=0;
    fTrackHeed->SetParticle(particleName);
    fTrackHeed->SetEnergy(eKin_eV);
    fTrackHeed->NewTrack(x_cm, y_cm, z_cm, t, dx, dy, dz);
    double xcl, ycl, zcl, tcl, ecl, extra;
    int ncl = 0;
    while (fTrackHeed->GetCluster(xcl, ycl, zcl, tcl, ncl, ecl, extra)) {
    // Retrieve the electrons of the cluster.
        for (int i = 0; i < ncl; ++i) {
            double x, y, z, t, e, dx, dy, dz;
            fTrackHeed->GetElectron(i, x, y, z, t, e, dx, dy, dz);
            GasBoxHit* gbh = new GasBoxHit();
            gbh->SetPos(G4ThreeVector(x*CLHEP::cm,y*CLHEP::cm,z*CLHEP::cm));
            gbh->SetTime(t);
            fGasBoxSD->InsertGasBoxHit(gbh);
            Drift(x,y,z,t);
        }
    }
    PlotTrack();
}



void HeedNewTrackModel::ProcessEvent(){

}

void HeedNewTrackModel::Reset(){

}