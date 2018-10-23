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

class G4VPhysicalVolume;
class DetectorConstruction;
class DegradMessenger;

typedef std::pair<double, double> EnergyRange_keV;
typedef std::multimap<const G4String, EnergyRange_keV> MapParticlesEnergy;

class DegradModel : public G4VFastSimulationModel {
 public:
  //-------------------------
  // Constructor, destructor
  //-------------------------
  DegradModel(G4String, G4Region*,DetectorConstruction*);
  ~DegradModel();


  virtual G4bool IsApplicable(const G4ParticleDefinition&);
  virtual G4bool ModelTrigger(const G4FastTrack&);
  virtual void DoIt(const G4FastTrack&, G4FastStep&);
  void Initialise();
  void InitialisePhysics();
  
  
  /*The following public methods are user-dependent*/

  //This method is called after each event, to record the relevant data
  void ProcessEvent();
  //This method is called at the beginning of an event to reset some variables of the class
  void Reset();
  
  /*Getters and Setters*/

  

 private:
  void AddParticleName(const G4String particleName,double ekin_min_keV,double ekin_max_keV);
  G4bool FindParticleName(G4String name);
  G4bool FindParticleNameEnergy(G4String name,double ekin_keV);
  void Run(G4String particleName, double ekin_keV, double t, double x_cm,
            double y_cm, double z_cm, double dx, double dy, double dz);
  DetectorConstruction* detCon;
  DegradMessenger* fDegradMessenger;

  MapParticlesEnergy* fMapParticlesEnergy;
  
  /*The following private methods and variables are user-dependent*/

  
  
};

#endif /* DegradModel_H_ */
