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
// $Id: XenonHit.hh 69706 2013-05-13 09:12:40Z gcosmo $
//
/// \file XenonHit.hh
/// \brief Definition of the XenonHit class

#ifndef XenonHit_h
#define XenonHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

/// Tracker hit class
///
/// It defines data members to store the trackID, FibberNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fFibberNB, fEdep, fPos

class XenonHit : public G4VHit
{
  public:
    XenonHit();
    XenonHit(const XenonHit&);
    virtual ~XenonHit();

    // operators
    const XenonHit& operator=(const XenonHit&);
    G4int operator==(const XenonHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
    void SetTime      (G4double t){ fTime = t; };
    void SetPhotonEnergy (G4double e){ fEnergy = e; };

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4ThreeVector GetPos() const { return fPos; };
     G4double GetTime() const     { return fTime; };
      G4double GetPhotonEnergy() const     { return fEnergy; };

  private:

      G4int         fTrackID;
      G4double      fTime;
      G4ThreeVector fPos;
      G4double      fEnergy;
      
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<XenonHit> XenonHitsCollection;

extern G4ThreadLocal G4Allocator<XenonHit>* XenonHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* XenonHit::operator new(size_t)
{
  if(!XenonHitAllocator)
      XenonHitAllocator = new G4Allocator<XenonHit>;
  return (void *) XenonHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void XenonHit::operator delete(void *hit)
{
  XenonHitAllocator->FreeSingle((XenonHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
