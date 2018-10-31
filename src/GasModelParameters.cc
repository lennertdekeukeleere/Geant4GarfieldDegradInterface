#include "GasModelParameters.hh"
#include "DegradModel.hh"
#include "HeedInterfaceModel.hh"
#include "HeedOnlyModel.hh"
#include "GasModelParametersMessenger.hh"
#include "DetectorConstruction.hh"

GasModelParameters::GasModelParameters(DetectorConstruction* dc){
	fMessenger = new GasModelParametersMessenger(this);
	dc->SetGasModelParameters(this);
}


void GasModelParameters::AddParticleNameHeedOnly(const G4String particleName,double ekin_min_keV,double ekin_max_keV){
    if (ekin_min_keV >= ekin_max_keV) {
        return;
    }
    fMapParticlesEnergyHeedOnly.insert(
                                std::make_pair(particleName, std::make_pair(ekin_min_keV, ekin_max_keV)));
    G4cout << "HeedOnly: Particle added: " << ekin_min_keV << " " << ekin_max_keV << G4endl;
}

void GasModelParameters::AddParticleNameHeedInterface(const G4String particleName,double ekin_min_keV,double ekin_max_keV){
    if (ekin_min_keV >= ekin_max_keV) {
        return;
    }
    fMapParticlesEnergyHeedInterface.insert(
                                std::make_pair(particleName, std::make_pair(ekin_min_keV, ekin_max_keV)));
    G4cout << "HeedInterface: Particle added: " << ekin_min_keV << " " << ekin_max_keV << G4endl;
}
