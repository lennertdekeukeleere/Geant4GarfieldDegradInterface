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
// $Id: garfExcHit.hh 69706 2013-05-13 09:12:40Z gcosmo $
//
/// \file garfExcHit.hh
/// \brief Definition of the garfExcHit class

#ifndef garfExcHit_h
#define garfExcHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

/// Tracker hit class

class garfExcHit : public G4VHit
{
  public:
    garfExcHit();
    garfExcHit(const garfExcHit&);
    virtual ~garfExcHit();

    // operators
    const garfExcHit& operator=(const garfExcHit&);
    G4int operator==(const garfExcHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();
    G4ThreeVector GetPos();
    G4double GetTime();

    // Set methods
    void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
    void SetTime      (G4double t){ fTime = t; };

    // Get methods
    G4ThreeVector GetPos() const { return fPos; };
     G4double GetTime() const     { return fTime; };

  private:

      G4double      fTime;
      G4ThreeVector fPos;
      
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<garfExcHit> garfExcHitsCollection;

extern G4ThreadLocal G4Allocator<garfExcHit>* garfExcHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* garfExcHit::operator new(size_t)
{
  if(!garfExcHitAllocator)
      garfExcHitAllocator = new G4Allocator<garfExcHit>;
  return (void *) garfExcHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void garfExcHit::operator delete(void *hit)
{
  garfExcHitAllocator->FreeSingle((garfExcHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
