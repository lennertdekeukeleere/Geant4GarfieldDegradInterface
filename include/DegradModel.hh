/*
 * DegradModel.h
 *
 *  Created on: Apr 9, 2014
 *      Author: dpfeiffe
 */

#ifndef DEGRADMODEL_H_
#define DEGRADMODEL_H_

#include "G4ThreeVector.hh"
#include "G4VFastSimulationModel.hh"
#include "GasModelParameters.hh"
#include "GasBoxSD.hh"

class G4VPhysicalVolume;
class DetectorConstruction;
class GasBoxSD;


class DegradModel : public G4VFastSimulationModel {
 public:
  //-------------------------
  // Constructor, destructor
  //-------------------------
  DegradModel(GasModelParameters*, G4String, G4Region*,DetectorConstruction*,GasBoxSD*);
  ~DegradModel();


  virtual G4bool IsApplicable(const G4ParticleDefinition&);
  virtual G4bool ModelTrigger(const G4FastTrack&);
  virtual void DoIt(const G4FastTrack&, G4FastStep&);
 inline G4bool FindParticleName(G4String s){if(s=='e-') return true; return false;};

 private:
  void GetElectronsFromDegrad(G4FastStep& fastStep,G4ThreeVector degradPos,G4double degradTime);
  
  G4double thermalE;
  DetectorConstruction* detCon;
  GasBoxSD* fGasBoxSD;
 


  
  
};

#endif /* DegradModel_H_ */
