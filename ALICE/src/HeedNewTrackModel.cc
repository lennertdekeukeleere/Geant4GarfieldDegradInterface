/*
 * HeedNewTrackModel.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: dpfeiffe
 */
#include <iostream>
#include "HeedNewTrackModel.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "GasModelParameters.hh"
#include "GasBoxSD.hh"
#include "GasBoxHit.hh"
#include "G4SDManager.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4VVisManager.hh"
#include "G4FieldTrack.hh"
#include "G4FieldTrackUpdator.hh"
#include "G4PathFinder.hh"
#include "G4FastStep.hh"
#include "G4FastTrack.hh"


#include "G4AutoLock.hh"
namespace{G4Mutex aMutex = G4MUTEX_INITIALIZER;}

// HeedNewTrackModel derives from the HeedModel Class and uses the GasModelParameters Class to set some user-defined veriables
HeedNewTrackModel::HeedNewTrackModel(GasModelParameters* gmp,G4String modelName, G4Region* envelope,DetectorConstruction* dc, GasBoxSD* sd)
    : HeedModel(modelName, envelope,dc,sd)	{
        G4cout << "Copying the particle map" << G4endl;
        G4cout << gmp->GetParticleNamesHeedNewTrack().size() << G4endl;
        fMapParticlesEnergy = gmp->GetParticleNamesHeedNewTrack();
        G4cout << "set the gas file" << G4endl;
        gasFile = gmp->GetGasFile();
        ionMobFile = gmp->GetIonMobilityFile();
        driftElectrons = gmp->GetDriftElectrons();
        trackMicro = gmp->GetTrackMicroscopic();
        createAval = gmp->GetCreateAvalancheMC();
        fVisualizeChamber = gmp->GetVisualizeChamber();
        fVisualizeSignal = gmp->GetVisualizeSignals();
        fVisualizeField = gmp->GetVisualizeField();
        driftRKF = gmp->GetDriftRKF();
        vPlaneHV = gmp->GetVoltagePlaneHV();
        vPlaneLow = gmp->GetVoltagePlaneLow();
        vAnodeWires = gmp->GetVoltageAnodeWires();
        vCathodeWires = gmp->GetVoltageCathodeWires();
        vGate = gmp->GetVoltageGate();
        vDeltaGate = gmp->GetVoltageDeltaGate();
        name="HeedNewTrackModel";
        InitialisePhysics();
    }

HeedNewTrackModel::~HeedNewTrackModel() {}

//This method is called in the DoIt-method in parent class HeedModel
void HeedNewTrackModel::Run(G4FastStep& fastStep,const G4FastTrack& fastTrack, G4String particleName, double ekin_keV, double t, double x_cm,
            double y_cm, double z_cm, double dx, double dy, double dz){
    double ekin_eV = ekin_keV * 1000;
    fTrackHeed->EnableDebugging();
    fTrackHeed->SetParticle(particleName);
    fTrackHeed->SetEnergy(ekin_eV);
    fTrackHeed->NewTrack(x_cm, y_cm, z_cm, t, dx, dy, dz);
    double xcl, ycl, zcl, tcl, ecl, extra;
    int ncl = 0;
    while (fTrackHeed->GetCluster(xcl, ycl, zcl, tcl, ncl, ecl, extra)) {
        ekin_eV-=ecl;
        std::cout << "New Cluster: " << xcl << " " << ycl << " " << zcl << std::endl;
    // Retrieve the electrons of the cluster.
        for (int i = 0; i < ncl; ++i) {
            double x, y, z, te, ee, dxe, dye, dze;
            fTrackHeed->GetElectron(i, x, y, z, te, ee, dxe, dye, dze);
            GasBoxHit* gbh = new GasBoxHit();
            gbh->SetPos(G4ThreeVector(x*CLHEP::cm,y*CLHEP::cm,z*CLHEP::cm));
            gbh->SetTime(te);
            fGasBoxSD->InsertGasBoxHit(gbh);
            if(G4VVisManager::GetConcreteInstance() && i % 100 == 0)
                Drift(x,y,z,te);
        }
    }
    PlotTrack();
    // try to estimate the exit location of the high energy particle out of the gas volume from the initial location and momentum direction and update the track parameters: location, direction, energy
    G4Track track = * fastTrack.GetPrimaryTrack();
    G4FieldTrack aFieldTrack( '0' );
    G4FieldTrackUpdator::Update( &aFieldTrack, &track );
    
    G4double retSafety = -1.0;
    ELimited retStepLimited;
    G4FieldTrack endTrack( 'a' );
    G4double currentMinimumStep = 10.0*m;  // Temporary: change that to sth connected
    // to particle momentum.
    G4PathFinder* fPathFinder = G4PathFinder::GetInstance();
    /*G4double lengthAlongCurve = */
    fPathFinder->ComputeStep( aFieldTrack,
                             currentMinimumStep,
                             0,
                             fastTrack.GetPrimaryTrack()->GetCurrentStepNumber(),
                             retSafety,
                             retStepLimited,
                             endTrack,
                             fastTrack.GetPrimaryTrack()->GetVolume() );
    
    // Place the particle at the tracking detector exit
    // (at the place it would reach without the change of its momentum).
    fastStep.ProposePrimaryTrackFinalPosition( endTrack.GetPosition(), false );
    G4cout << "Particle location: " << endTrack.GetPosition() << G4endl;
    fastStep.SetPrimaryTrackFinalKineticEnergyAndDirection(ekin_eV*eV, G4ThreeVector(dx,dy,dz),false);
    fastStep.SetTotalEnergyDeposited((ekin_keV*1000-ekin_eV)*eV);
    std::cout << "Particle Tracked out of the gas volume" << std::endl;
}



void HeedNewTrackModel::ProcessEvent(){

}

void HeedNewTrackModel::Reset(){

}
