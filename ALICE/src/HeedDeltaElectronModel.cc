/*
 * HeedDeltaElectronModel.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: dpfeiffe
 */
#include <iostream>
#include "HeedDeltaElectronModel.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "GasModelParameters.hh"
#include "GasBoxSD.hh"
#include "G4VVisManager.hh"
#include "G4FastStep.hh"
#include "G4FastTrack.hh"

#include "G4AutoLock.hh"
namespace{G4Mutex aMutex = G4MUTEX_INITIALIZER;}

// HeedDeltaElectronModel derives from the HeedModel Class and uses the GasModelParameters Class to set some user-defined veriables
HeedDeltaElectronModel::HeedDeltaElectronModel(GasModelParameters* gmp,G4String modelName, G4Region* envelope,DetectorConstruction* dc, GasBoxSD* sd)
    : HeedModel(modelName, envelope,dc,sd) {
        fMapParticlesEnergy = gmp->GetParticleNamesHeedDeltaElectron();
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
        name="HeedDeltaElectronModel";
        InitialisePhysics();
    }

HeedDeltaElectronModel::~HeedDeltaElectronModel() {}

//This method is called in the DoIt-method in parent class HeedModel
void HeedDeltaElectronModel::Run(G4FastStep& fastStep,const G4FastTrack& fastTrack, G4String particleName, double ekin_keV, double t, double x_cm,
            double y_cm, double z_cm, double dx, double dy, double dz){
    double eKin_eV = ekin_keV * 1000;
    int nc = 0, ni=0;
    G4cout << "Run Interface" << G4endl;
    G4cout << "Electron energy(in eV): " << eKin_eV << G4endl;
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
    for (int cl = 0; cl < nc; cl++) {
        double xe, ye, ze, te;
        double ee, dxe, dye, dze;
        fTrackHeed->GetElectron(cl, xe, ye, ze, te, ee, dxe, dye, dze);
        GasBoxHit* gbh = new GasBoxHit();
        gbh->SetPos(G4ThreeVector(xe*CLHEP::cm,ye*CLHEP::cm,ze*CLHEP::cm));
        gbh->SetTime(te);
        fGasBoxSD->InsertGasBoxHit(gbh);
        if(G4VVisManager::GetConcreteInstance() && cl % 100 == 0)
            Drift(xe,ye,ze,te);
    }
    PlotTrack();
    fastStep.KillPrimaryTrack();
    fastStep.SetPrimaryTrackPathLength(0.0);
    fastStep.SetTotalEnergyDeposited(ekin_keV*keV);

}

void HeedDeltaElectronModel::ProcessEvent(){

}

void HeedDeltaElectronModel::Reset(){
  
}


