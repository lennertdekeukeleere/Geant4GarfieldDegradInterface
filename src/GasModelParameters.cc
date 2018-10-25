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

void GasModelParameters::AddParticleNameDegrad(const G4String particleName,double ekin_min_keV,double ekin_max_keV){
    if (ekin_min_keV >= ekin_max_keV) {
        return;
    }
    fMapParticlesEnergyDegrad.insert(
                                std::make_pair(particleName, std::make_pair(ekin_min_keV, ekin_max_keV)));
    G4cout << "Degrad: Particle added: " << ekin_min_keV << " " << ekin_max_keV << G4endl;
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

bool GasModelParameters::NoParticlesForModel(){
    return (fMapParticlesEnergyDegrad.empty() && fMapParticlesEnergyHeedOnly.empty() &&
            fMapParticlesEnergyHeedInterface.empty());
}

bool GasModelParameters::FindParticleName(G4String name){
    MapParticlesEnergy::iterator it;
    it = fMapParticlesEnergyDegrad.find(name);
    if (it != fMapParticlesEnergyDegrad.end()) {
        return true;
    }
    it = fMapParticlesEnergyHeedOnly.find(name);
    if (it != fMapParticlesEnergyHeedOnly.end()) {
        return true;
    }
    it = fMapParticlesEnergyHeedInterface.find(name);
    if (it != fMapParticlesEnergyHeedInterface.end()) {
        return true;
    }
    return false;
}
