//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: LXeTrajectory.cc 72349 2013-07-16 12:13:16Z gcosmo $
//
/// \file optical/LXe/src/LXeTrajectory.cc
/// \brief Implementation of the LXeTrajectory class
//
//
#include "DriftLineTrajectory.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "DriftLineTrajectoryPoint.hh"
#include "G4VProcess.hh"
G4ThreadLocal G4Allocator<DriftLineTrajectory>* DriftLineTrajectoryAllocator = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DriftLineTrajectory::DriftLineTrajectory()
{
  fpPointsContainer = new DriftLineTrajectoryPointContainer();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DriftLineTrajectory::DriftLineTrajectory(DriftLineTrajectory &right)
  :G4Trajectory(right)
{
  fpPointsContainer = new DriftLineTrajectoryPointContainer();
  for(size_t i=0;i<right.fpPointsContainer->size();++i) {
      DriftLineTrajectoryPoint* rightPoint
          = (DriftLineTrajectoryPoint*)((*(right.fpPointsContainer))[i]);
      fpPointsContainer->push_back(new DriftLineTrajectoryPoint(*rightPoint));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DriftLineTrajectory::~DriftLineTrajectory() {
	for(size_t i=0;i<fpPointsContainer->size();++i){
		delete  (*fpPointsContainer)[i];
	}
	fpPointsContainer->clear();

	delete fpPointsContainer;
}


void DriftLineTrajectory::AppendStep(G4ThreeVector pos, G4double t){
		fpPointsContainer->push_back(new DriftLineTrajectoryPoint(pos,t));
}
