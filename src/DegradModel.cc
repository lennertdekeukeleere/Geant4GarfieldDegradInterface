/*
 * DegradModel.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: dpfeiffe
 */
#include <iostream>
#include "DegradModel.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "GasModelParameters.hh"
#include "DetectorConstruction.hh"

DegradModel::DegradModel(GasModelParameters* gmp, G4String modelName, G4Region* envelope,DetectorConstruction* dc)
    : G4VFastSimulationModel(modelName, envelope),detCon(dc)	{
      fMapParticlesEnergy = new MapParticlesEnergy();
    }

DegradModel::~DegradModel() {}

G4bool DegradModel::IsApplicable(const G4ParticleDefinition& particleType) {
  G4String particleName = particleType.GetParticleName();
  if (FindParticleName(particleName))
    return true;
  return false;
}

G4bool DegradModel::ModelTrigger(const G4FastTrack& fastTrack) {
  G4double ekin = fastTrack.GetPrimaryTrack()->GetKineticEnergy();
  G4String particleName =
      fastTrack.GetPrimaryTrack()->GetParticleDefinition()->GetParticleName();
  if (FindParticleNameEnergy(particleName, ekin / keV))
		return true;
  return false;

}

void DegradModel::DoIt(const G4FastTrack& fastTrack, G4FastStep& fastStep) {

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


G4bool DegradModel::FindParticleName(G4String name) {
  MapParticlesEnergy::iterator it;
  it = fMapParticlesEnergy->find(name);
  if (it != fMapParticlesEnergy->end()) {
    return true;
  }
  return false;
}

G4bool DegradModel::FindParticleNameEnergy(G4String name,
                                             double ekin_keV) {
  MapParticlesEnergy::iterator it;
//  it = fMapParticlesEnergy->find(name);
  for (it=fMapParticlesEnergy->begin(); it!=fMapParticlesEnergy->end();++it) {
    if(it->first == name){
      EnergyRange_keV range = it->second;
      if (range.first <= ekin_keV && range.second >= ekin_keV) {
        return true;
      }
    }
  }
  return false;
}

