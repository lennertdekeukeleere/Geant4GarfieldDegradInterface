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
// $Id: WLSTrajectoryPoint.hh 72065 2013-07-05 09:54:59Z gcosmo $
//
/// \file optical/wls/include/WLSTrajectoryPoint.hh
/// \brief Definition of the WLSTrajectoryPoint class
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef WLSTrajectoryPoint_h_seen
#define WLSTrajectoryPoint_h_seen 1

#include "globals.hh"

#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4TrajectoryPoint.hh"

#include "G4StepStatus.hh"

class G4Track;
class G4Step;
class G4VProcess;

class DriftLineTrajectoryPoint : public G4TrajectoryPoint {

//--------
  public: // without description
//--------

// Constructor/Destructor

    DriftLineTrajectoryPoint();
    DriftLineTrajectoryPoint(G4ThreeVector, G4double);
    DriftLineTrajectoryPoint(const DriftLineTrajectoryPoint &right);
    virtual ~DriftLineTrajectoryPoint();

// Operators

    inline void *operator new(size_t);
    inline void operator delete(void *aTrajectoryPoint);
    inline int operator==(const DriftLineTrajectoryPoint& right) const
    { return (this==&right); };

    virtual std::vector<G4AttValue>* CreateAttValues() const;

    inline G4double GetTime() const { return fTime; };
//---------
  private:
//---------

    G4double fTime;
};

extern G4ThreadLocal G4Allocator<DriftLineTrajectoryPoint>* DriftLineTrajectoryPointAllocator;

inline void* DriftLineTrajectoryPoint::operator new(size_t)
{
    if(!DriftLineTrajectoryPointAllocator)
      DriftLineTrajectoryPointAllocator = new G4Allocator<DriftLineTrajectoryPoint>;
    return (void *) DriftLineTrajectoryPointAllocator->MallocSingle();
}

inline void DriftLineTrajectoryPoint::operator delete(void *aTrajectoryPoint)
{
    DriftLineTrajectoryPointAllocator->FreeSingle(
        (DriftLineTrajectoryPoint *) aTrajectoryPoint);
}

#endif
