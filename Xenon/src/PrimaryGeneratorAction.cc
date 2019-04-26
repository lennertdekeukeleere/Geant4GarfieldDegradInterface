//
//  PrimaryGeneratorAction.cpp
//  Xenon
//
//  Created by Lennert De Keukeleere on 25/10/2018.
//

#include "PrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4RandomDirection.hh"
#include "G4RunManager.hh"
#include "DetectorConstruction.hh"
#include "RunAction.hh"
#include "G4ThreeVector.hh"
#include "G4Geantino.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(){
    particleGun = new G4GeneralParticleSource();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete particleGun;
    G4cout << "Deleting PrimaryGeneratorAction" << G4endl;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    particleGun->GeneratePrimaryVertex(anEvent);
}

