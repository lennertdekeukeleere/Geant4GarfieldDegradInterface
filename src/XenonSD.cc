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
// $Id: XenonSD.cc 87359 2014-12-01 16:04:27Z gcosmo $

//FROM EXAMPLE B2a
//
/// \file XenonSD.cc
/// \brief Implementation of the XenonSD class

#include "XenonSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XenonSD::XenonSD(const G4String& name) 
 : G4VSensitiveDetector(name),
   fHitsXenonCollection(NULL),fGarfieldExcitationHitsCollection(NULL)
{
  collectionName.insert("XenonHC");
  collectionName.insert("GExcHC");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

XenonSD::~XenonSD() 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XenonSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection

  fHitsXenonCollection = new XenonHitsCollection(SensitiveDetectorName, collectionName[0]);
  fGarfieldExcitationHitsCollection = new XenonHitsCollection(SensitiveDetectorName, collectionName[1]);

  // Add this collection in hce

  G4int XHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  G4int GEXCHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[1]);
  hce->AddHitsCollection( XHCID, fHitsXenonCollection );
  hce->AddHitsCollection( GEXCHCID, fGarfieldExcitationHitsCollection );

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool XenonSD::ProcessHits(G4Step* aStep, 
                                     G4TouchableHistory*)
{  
  //just interested in the interaction point
  if (trackID==1){
    XenonHit* newHit = new XenonHit();
    newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
    //We must collect PostStep. PreStep will collect the xenon surface
    newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
    newHit->SetTime(aStep->GetTrack()->GetGlobalTime());
    newHit->SetPhotonEnergy(aStep->GetTrack()->GetVertexKineticEnergy()*1000);//*1000 beacuse the G4 energy comes in MeV

    fHitsCollection->insert( newHit );
    newHit->Print();
}
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void XenonSD::EndOfEvent(G4HCofThisEvent*)
{
  if ( verboseLevel>1 ) { 
     G4int nofHits = fHitsCollection->entries();
     G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits 
            << " hits in the gas: " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
  
  system("rm DEGRAD.OUT");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
