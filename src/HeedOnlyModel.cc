/*
 * HeedOnlyModel.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: dpfeiffe
 */
#include <iostream>
#include "HeedOnlyModel.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "GasModelParameters.hh"

#include "G4AutoLock.hh"
namespace{G4Mutex aMutex = G4MUTEX_INITIALIZER;}

HeedOnlyModel::HeedOnlyModel(GasModelParameters* gmp,G4String modelName, G4Region* envelope,DetectorConstruction* dc)
    : HeedModel(modelName, envelope,dc)	{
        fMapParticlesEnergy = new MapParticlesEnergy();
    }

HeedOnlyModel::~HeedOnlyModel() {}

void HeedOnlyModel::Run(G4String particleName, double ekin_keV, double t, double x_cm,
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
                                           dz, nc, ni);
    }
    for (int cl = 0; cl < nc; cl++) {
        double xe, ye, ze, te;
        double ee, dxe, dye, dze;
        fTrackHeed->GetElectron(cl, xe, ye, ze, te, ee, dxe, dye, dze);
        double newTime = te;
        if (newTime < t) {
            newTime += t;
        }
        AddSecondary(ee, newTime, xe, ye, ze, dxe, dye, dze,0,0);
        if(driftElectrons)
            Drift(xe,ye,ze,newTime);
}

void HeedOnlyModel::ProcessEvent(){

}

void HeedOnlyModel::Reset(){

}
