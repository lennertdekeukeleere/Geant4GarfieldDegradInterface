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
// $Id: WLSTrajectoryPoint.cc 72065 2013-07-05 09:54:59Z gcosmo $
//
/// \file optical/wls/src/WLSTrajectoryPoint.cc
/// \brief Implementation of the WLSTrajectoryPoint class
//
//
#include "DriftLineTrajectoryPoint.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4StepStatus.hh"
#include "G4UnitsTable.hh"
#include "G4AttValue.hh"


G4ThreadLocal G4Allocator<DriftLineTrajectoryPoint>* DriftLineTrajectoryPointAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DriftLineTrajectoryPoint::DriftLineTrajectoryPoint()
      : fTime(0) { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DriftLineTrajectoryPoint::DriftLineTrajectoryPoint(G4ThreeVector pos, G4double t)
      : G4TrajectoryPoint(pos), fTime(t){}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DriftLineTrajectoryPoint::DriftLineTrajectoryPoint(const DriftLineTrajectoryPoint &right)
    : G4TrajectoryPoint(right)
{
     fTime = right.fTime;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DriftLineTrajectoryPoint::~DriftLineTrajectoryPoint() { }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* DriftLineTrajectoryPoint::CreateAttValues() const
{
  std::vector<G4AttValue>* values = new std::vector<G4AttValue>;
  values->push_back(G4AttValue("Pos",G4BestUnit(GetPosition(),"Length"),""));
  values->push_back
    (G4AttValue("PreT",G4BestUnit(fTime,"Time"),""));
  values->push_back
    (G4AttValue("PostT",G4BestUnit(fTime,"Time"),""));

#ifdef G4ATTDEBUG
  G4cout << G4AttCheck(values,GetAttDefs());
#endif

  return values;
}
