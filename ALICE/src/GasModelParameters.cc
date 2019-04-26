#include "GasModelParameters.hh"
#include "HeedDeltaElectronModel.hh"
#include "HeedNewTrackModel.hh"
#include "GasModelParametersMessenger.hh"
#include "DetectorConstruction.hh"

GasModelParameters::GasModelParameters(){
	fMessenger = new GasModelParametersMessenger(this);
}

//Add particles (with energy range) to be included in the HeedNewTrack Model
void GasModelParameters::AddParticleNameHeedNewTrack(const G4String particleName,double ekin_min_keV,double ekin_max_keV){
    if (ekin_min_keV >= ekin_max_keV) {
        return;
    }
    fMapParticlesEnergyHeedNewTrack.insert(
                                std::make_pair(particleName, std::make_pair(ekin_min_keV, ekin_max_keV)));
    G4cout << "HeedNewTrack: Particle added: " << ekin_min_keV << " " << ekin_max_keV << G4endl;
}
//Add particles (with energy range) to be included in the HeedDeltaElectron Model
void GasModelParameters::AddParticleNameHeedDeltaElectron(const G4String particleName,double ekin_min_keV,double ekin_max_keV){
    if (ekin_min_keV >= ekin_max_keV) {
        return;
    }
    fMapParticlesEnergyHeedDeltaElectron.insert(
                                std::make_pair(particleName, std::make_pair(ekin_min_keV, ekin_max_keV)));
    G4cout << "HeedDeltaElectron: Particle added: " << ekin_min_keV << " " << ekin_max_keV << G4endl;
}
