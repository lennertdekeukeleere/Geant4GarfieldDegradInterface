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
// $Id: GarfieldExcitationHit.hh 69706 2013-05-13 09:12:40Z gcosmo $
//
/// \file GarfieldExcitationHit.hh
/// \brief Definition of the GarfieldExcitationHit class

#ifndef GarfieldExcitationHit_h
#define GarfieldExcitationHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

/// Tracker hit class

class GarfieldExcitationHit : public G4VHit
{
  public:
    GarfieldExcitationHit();
    GarfieldExcitationHit(const GarfieldExcitationHit&);
    virtual ~GarfieldExcitationHit();

    // operators
    const GarfieldExcitationHit& operator=(const GarfieldExcitationHit&);
    G4int operator==(const GarfieldExcitationHit&) const;

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

using GarfieldExcitationHitsCollection=G4THitsCollection<GarfieldExcitationHit>;

extern G4ThreadLocal G4Allocator<GarfieldExcitationHit>* GarfieldExcitationHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* GarfieldExcitationHit::operator new(size_t)
{
  if(!GarfieldExcitationHitAllocator)
      GarfieldExcitationHitAllocator = new G4Allocator<GarfieldExcitationHit>;
  return (void *) GarfieldExcitationHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void GarfieldExcitationHit::operator delete(void *hit)
{
  GarfieldExcitationHitAllocator->FreeSingle((GarfieldExcitationHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
