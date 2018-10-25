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


HeedInterfaceModel::HeedInterfaceModel(GasModelParameters*,G4String modelName, G4Region* envelope,DetectorConstruction* dc)
    : HeedModel(modelName, envelope,dc) {
        fMapParticlesEnergy = new MapParticlesEnergy();
    }

HeedInterfaceModel::~HeedInterfaceModel() {}

void HeedInterfaceModel::Run(G4String particleName, double ekin_keV, double t, double x_cm,
            double y_cm, double z_cm, double dx, double dy, double dz){

}

void HeedInterfaceModel::ProcessEvent(){

}

void HeedInterfaceModel::Reset(){
  
}


