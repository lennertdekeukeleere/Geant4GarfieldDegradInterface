/*
 * HeedModel.h
 *
 *  Created on: Apr 9, 2014
 *      Author: dpfeiffe
 */

#ifndef HEEDMODEL_H_
#define HEEDMODEL_H_

#include "SolidTube.hh"      //Geometry
#include "ComponentAnalyticField.hh"  //Garfield field
#include "G4ThreeVector.hh"
#include "ComponentVoxel.hh"
#include "AvalancheMicroscopic.hh"
#include "AvalancheMC.hh"
#include "DriftLineRKF.hh"
#include "ViewCell.hh"      //Visualization
#include "TCanvas.h"
#include "ViewDrift.hh"
#include "ViewSignal.hh"
#include "ViewField.hh"
#include "G4VFastSimulationModel.hh"
#include "MediumMagboltz.hh"
#include "TrackHeed.hh"
#include "GeometrySimple.hh"
#include "GasModelParameters.hh"
#include "GasBoxSD.hh"


class G4VPhysicalVolume;
class DetectorConstruction;
class HeedMessenger;
class G4FastStep;
class G4FastTrack;

class HeedModel : public G4VFastSimulationModel {
 public:
  //-------------------------
  // Constructor, destructor
  //-------------------------
  HeedModel(G4String, G4Region*,DetectorConstruction*,GasBoxSD*);
  ~HeedModel();


  virtual G4bool IsApplicable(const G4ParticleDefinition&);
  virtual G4bool ModelTrigger(const G4FastTrack&);
  virtual void DoIt(const G4FastTrack&, G4FastStep&);
  
  /*The following public methods are user-dependent*/

  //This method is called after each event, to record the relevant data
  virtual void ProcessEvent() = 0;
  //This method is called at the beginning of an event to reset some variables of the class
  virtual void Reset() = 0;
  G4bool FindParticleName(G4String name);
  G4bool FindParticleNameEnergy(G4String name,double ekin_keV);

 protected:
  void InitialisePhysics();
  virtual void Run(G4FastStep& fastStep,const G4FastTrack& fastTrack, G4String particleName, double ekin_keV, double t, double x_cm, double y_cm, double z_cm, double dx, double dy, double dz) = 0;
  void PlotTrack();
  void Drift(double,double, double, double);
  DetectorConstruction* detCon;
  HeedMessenger* fHeedMessenger;

  MapParticlesEnergy fMapParticlesEnergy;

  G4String gasFile;
  G4String ionMobFile;
  
  bool driftElectrons;
  bool trackMicro;
  bool createAval;
  bool fVisualizeChamber;
  bool fVisualizeSignal;
  bool fVisualizeField;
  bool driftRKF;

  double vPlaneHV;
  double vPlaneLow;
  double vAnodeWires;
  double vCathodeWires;
  double vGate;
  double vDeltaGate;

  Garfield::TrackHeed* fTrackHeed;
  GasBoxSD* fGasBoxSD;

  const char* name;
  
  /*The following private methods and variables are user-dependent*/
 private:
  void makeGas();
  void buildBox();
  void loadComsol();
  void BuildCompField();
  void BuildSensor();
  void SetTracking();
  void CreateChamberView();
  void CreateSignalView();
  void CreateFieldView();

  Garfield::MediumMagboltz* fMediumMagboltz;
  Garfield::Sensor* fSensor;
//  Garfield::TrackHeed* fTrackHeed;
  Garfield::GeometrySimple* geo;
  Garfield::SolidTube* box;
  Garfield::ComponentVoxel* voxfield;
  Garfield::ComponentAnalyticField* comp;
  Garfield::AvalancheMC* fDrift;
  Garfield::DriftLineRKF* fDriftRKF;
  Garfield::AvalancheMicroscopic* fAvalanche;
  TCanvas* fChamber;
  TCanvas* fSignal;
  TCanvas* fField;
  Garfield::ViewCell* cellView;
  Garfield::ViewDrift* viewDrift;
  Garfield::ViewSignal* viewSignal;
  Garfield::ViewField* viewField;
  
};

#endif /* HeedModel_H_ */
