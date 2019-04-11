#include "GasModelParameters.hh"
#include "DegradModel.hh"
#include "HeedInterfaceModel.hh"
#include "HeedOnlyModel.hh"
#include "GasModelParametersMessenger.hh"
#include "DetectorConstruction.hh"

GasModelParameters::GasModelParameters(){
	fMessenger = new GasModelParametersMessenger(this);
}

//Add particles (with energy range) to be included in the HeedOnly Model
void GasModelParameters::AddParticleNameHeedOnly(const G4String particleName,double ekin_min_keV,double ekin_max_keV){
    if (ekin_min_keV >= ekin_max_keV) {
        return;
    }
    fMapParticlesEnergyHeedOnly.insert(
                                std::make_pair(particleName, std::make_pair(ekin_min_keV, ekin_max_keV)));
    G4cout << "HeedOnly: Particle added: " << ekin_min_keV << " " << ekin_max_keV << G4endl;
}
//Add particles (with energy range) to be included in the HeedInterface Model
void GasModelParameters::AddParticleNameHeedInterface(const G4String particleName,double ekin_min_keV,double ekin_max_keV){
    if (ekin_min_keV >= ekin_max_keV) {
        return;
    }
    fMapParticlesEnergyHeedInterface.insert(
                                std::make_pair(particleName, std::make_pair(ekin_min_keV, ekin_max_keV)));
    G4cout << "HeedInterface: Particle added: " << ekin_min_keV << " " << ekin_max_keV << G4endl;
}
