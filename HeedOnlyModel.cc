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
#include "HeedMessenger.hh"

#include "G4SystemOfUnits.hh"

const static G4double torr = 1. / 760. * atmosphere;


HeedOnlyModel::HeedOnlyModel(G4String modelName, G4Region* envelope, DetectorConstruction* dc)
    : HeedModel(modelName, envelope,dc)	{}

HeedOnlyModel::~HeedOnlyModel() {}


G4bool HeedOnlyModel::ModelTrigger(const G4FastTrack& fastTrack) {
  G4double ekin = fastTrack.GetPrimaryTrack()->GetKineticEnergy();
  G4String particleName =
      fastTrack.GetPrimaryTrack()->GetParticleDefinition()->GetParticleName();
  if (FindParticleNameEnergy(particleName, ekin / keV))
		return true;
  return false;

}

void HeedOnlyModel::DoIt(const G4FastTrack& fastTrack, G4FastStep& fastStep) {

  G4ThreeVector localdir = fastTrack.GetPrimaryTrackLocalDirection();

  G4ThreeVector worldPosition = fastTrack.GetPrimaryTrack()->GetPosition();

  G4double ekin = fastTrack.GetPrimaryTrack()->GetKineticEnergy();
  G4double time = fastTrack.GetPrimaryTrack()->GetGlobalTime();
  G4String particleName =
      fastTrack.GetPrimaryTrack()->GetParticleDefinition()->GetParticleName();

  fastStep.KillPrimaryTrack();
  fastStep.SetPrimaryTrackPathLength(0.0);
  
  Run(particleName, ekin/keV, time, worldPosition.x() / CLHEP::cm,
      worldPosition.y() / CLHEP::cm, worldPosition.z() / CLHEP::cm,
      localdir.x(), localdir.y(), localdir.z());
  
  fastStep.SetTotalEnergyDeposited(ekin);


}

void HeedOnlyModel::Initialise() {
  AddParticleName("e-", 1 * eV / keV, ELimit / keV);
  AddParticleName("gamma",1 * eV / keV, 1e+8 *MeV /keV);
}

