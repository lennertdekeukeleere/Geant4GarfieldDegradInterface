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

const static G4double torr = 1. / 760. * atmosphere;


HeedOnlyModel::HeedOnlyModel(G4String modelName, G4Region* envelope, DetectorConstruction* dc)
    : HeedModel(modelName, envelope,dc)	{}

HeedOnlyModel::~HeedOnlyModel() {}

void HeedOnlyModel::Run(G4String particleName, double ekin_keV, double t, double x_cm,
            double y_cm, double z_cm, double dx, double dy, double dz){

}

void HeedOnlyModel::ProcessEvent(){

}

void HeedOnlyModel::Reset(){

}
