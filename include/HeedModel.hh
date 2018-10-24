/*
 * HeedModel.h
 *
 *  Created on: Apr 9, 2014
 *      Author: dpfeiffe
 */

#ifndef HEEDMODEL_H_
#define HEEDMODEL_H_

#include "SolidBox.hh"      //Geometry
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


class G4VPhysicalVolume;
class DetectorConstruction;
class HeedMessenger;

typedef std::pair<double, double> EnergyRange_keV;
typedef std::multimap<const G4String, EnergyRange_keV> MapParticlesEnergy;

class HeedModel : public G4VFastSimulationModel {
 public:
  //-------------------------
  // Constructor, destructor
  //-------------------------
  HeedModel(G4String, G4Region*,DetectorConstruction*);
  ~HeedModel();


  virtual G4bool IsApplicable(const G4ParticleDefinition&);
  virtual G4bool ModelTrigger(const G4FastTrack&);
  virtual void DoIt(const G4FastTrack&, G4FastStep&);
  void AddParticleName(const G4String particleName,double ekin_min_keV,double ekin_max_keV);
  
  /*The following public methods are user-dependent*/

  //This method is called after each event, to record the relevant data
  virtual void ProcessEvent() = 0;
  //This method is called at the beginning of an event to reset some variables of the class
  virtual void Reset() = 0;
  
  /*Getters and Setters*/

  //Name of the Magboltz file to be used (if needed)
  inline void SetGasFile(G4String s) { gasFile = s;};
  //Name of the Ion mobility file (if needed)
  inline void SetIonMobilityFile(G4String s) { ionMobFile = s; };
  //Determines if the electrons are drifted, or only primary ionization is simulated
  inline void SetDriftElectrons(G4bool b) { driftElectrons = b; };
  inline bool GetDriftElectrons(){return driftElectrons;};
  inline void SetVoltagePlaneHV(G4double v){vPlaneHV = v;};
  inline void SetVoltagePlaneLow(G4double v){vPlaneLow = v;};
  inline void SetVoltageAnodeWires(G4double v){vAnodeWires = v;};
  inline void SetVoltageCathodeWires(G4double v){vCathodeWires = v;};
  inline void SetVoltageGate(G4double v){vGate = v;};
  inline void SetVoltageDeltaGate(G4double v){vDeltaGate = v;};
  inline void SetTrackMicroscopic(bool b){trackMicro=b;};
  inline void SetCreateAvalancheMC(bool b){createAval=b;};
  inline void SetVisualizeChamber(bool b){fVisualizeChamber = b;};
  inline void SetVisualizeSignals(bool b){fVisualizeSignal = b;};
  inline void SetVisualizeField(bool b){fVisualizeField = b;};
  inline void SetDriftRKF(bool b){driftRKF=b;};

 protected:
  void InitialisePhysics();
  virtual void Run(G4String particleName, double ekin_keV, double t, double x_cm,
            double y_cm, double z_cm, double dx, double dy, double dz) = 0;
  void Drift();
  G4bool FindParticleName(G4String name);
  G4bool FindParticleNameEnergy(G4String name,double ekin_keV);
  DetectorConstruction* detCon;
  HeedMessenger* fHeedMessenger;

  MapParticlesEnergy* fMapParticlesEnergy;

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
  Garfield::SolidBox* box; 
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
