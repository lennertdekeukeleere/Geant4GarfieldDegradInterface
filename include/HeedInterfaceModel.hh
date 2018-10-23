/*
 * HeedInterfaceModel.h
 *
 *  Created on: Apr 9, 2014
 *      Author: dpfeiffe
 */

#ifndef HEEDINTERFACEMODEL_H_
#define HEEDINTERFACEMODEL_H_

#include "HeedModel.hh"

class G4VPhysicalVolume;
class DetectorConstruction;
class HeedInterfaceMessenger;

typedef std::pair<double, double> EnergyRange_keV;
typedef std::multimap<const G4String, EnergyRange_keV> MapParticlesEnergy;

class HeedInterfaceModel : public HeedModel {
 public:
  //-------------------------
  // Constructor, destructor
  //-------------------------
  HeedInterfaceModel(G4String, G4Region*,DetectorConstruction*);
  ~HeedInterfaceModel();


  virtual G4bool ModelTrigger(const G4FastTrack&);
  virtual void DoIt(const G4FastTrack&, G4FastStep&);
  void Initialise();
  

  /*The following public methods are user-dependent*/

  //This method is called after each event, to record the relevant data
  void ProcessEvent();
  //This method is called at the beginning of an event to reset some variables of the class
  void Reset();
  
  /*Getters and Setters*/

 private:
  void AddParticleName(const G4String particleName,double ekin_min_keV,double ekin_max_keV);
  G4bool FindParticleNameEnergy(G4String name,double ekin_keV);
  void Run(G4String particleName, double ekin_keV, double t, double x_cm,
            double y_cm, double z_cm, double dx, double dy, double dz);
  
  

  
};

#endif /* HeedInterfaceModel_H_ */
