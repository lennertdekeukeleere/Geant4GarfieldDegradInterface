/*
 * HeedModel.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: dpfeiffe
 */
#include <iostream>
#include "HeedModel.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "DetectorConstruction.hh"

#include "G4AutoLock.hh"
namespace{G4Mutex aMutex = G4MUTEX_INITIALIZER;}

const static G4double torr = 1. / 760. * atmosphere;


HeedModel::HeedModel(G4String modelName, G4Region* envelope,DetectorConstruction* dc)
: G4VFastSimulationModel(modelName, envelope), detCon(dc)	{
    }

HeedModel::~HeedModel() {}

G4bool HeedModel::IsApplicable(const G4ParticleDefinition& particleType) {
  G4String particleName = particleType.GetParticleName();
  return FindParticleName(particleName);
}

G4bool HeedModel::ModelTrigger(const G4FastTrack& fastTrack) {
  G4double ekin = fastTrack.GetPrimaryTrack()->GetKineticEnergy();
  G4String particleName =
      fastTrack.GetPrimaryTrack()->GetParticleDefinition()->GetParticleName();
  return FindParticleNameEnergy(particleName, ekin / keV);
}

void HeedModel::DoIt(const G4FastTrack& fastTrack, G4FastStep& fastStep) {

  G4ThreeVector localdir = fastTrack.GetPrimaryTrackLocalDirection();

  G4ThreeVector worldPosition = fastTrack.GetPrimaryTrack()->GetPosition();

  G4double ekin = fastTrack.GetPrimaryTrack()->GetKineticEnergy();
  G4double time = fastTrack.GetPrimaryTrack()->GetGlobalTime();
  G4String particleName =
      fastTrack.GetPrimaryTrack()->GetParticleDefinition()->GetParticleName();

  fastStep.KillPrimaryTrack();
	fastStep.SetPrimaryTrackPathLength(0.0);
  Run(particleName, ekin/keV, time, worldPosition.x() / CLHEP::cm,
      worldPosition.y() / CLHEP::cm, worldPosition.z() / CLHEP::cm,
      localdir.x(), localdir.y(), localdir.z());
	fastStep.SetTotalEnergyDeposited(ekin);
}

G4bool HeedModel::FindParticleName(G4String name) {
  MapParticlesEnergy::iterator it;
  it = fMapParticlesEnergy->find(name);
  if (it != fMapParticlesEnergy->end()) {
    return true;
  }
  return false;
}

G4bool HeedModel::FindParticleNameEnergy(G4String name,
                                             double ekin_keV) {
  MapParticlesEnergy::iterator it;
//  it = fMapParticlesEnergy->find(name);
  for (it=fMapParticlesEnergy->begin(); it!=fMapParticlesEnergy->end();++it) {
    if(it->first == name){
      EnergyRange_keV range = it->second;
      if (range.first <= ekin_keV && range.second >= ekin_keV) {
        return true;
      }
    }
  }
  return false;
}

void HeedModel::InitialisePhysics(){
  makeGas();
    
  buildBox();
  
  BuildCompField();
  
  BuildSensor();
  
  SetTracking();
  
  if(fVisualizeChamber) CreateChamberView();
  if(fVisualizeSignal) CreateSignalView();
  if(fVisualizeField) CreateFieldView();
}

void HeedModel::makeGas(){
  fMediumMagboltz = new Garfield::MediumMagboltz();
  double pressure = detCon->GetGasPressure()/torr;
  double temperature = detCon->GetTemperature()/kelvin;
  double addmixtPerc = detCon->GetAddmixturePercentage();
  G4String gasName = detCon->GetGasName();
  const std::string path = getenv("GARFIELD_HOME");
  if(gasName=="HeIso"){
    fMediumMagboltz->SetComposition("isobutane",addmixtPerc,"helium",
          100.-addmixtPerc);
    const double rPenning = 1;
    const double lambdaPenning = 0.;
    fMediumMagboltz->EnablePenningTransfer(rPenning, lambdaPenning, "helium");
  }
  else if(gasName=="ArCO2"){
    fMediumMagboltz->SetComposition("CO2",addmixtPerc,"Ar",
          100.-addmixtPerc);
    const double rPenning = 0.51;
    const double lambdaPenning = 0.;
    fMediumMagboltz->EnablePenningTransfer(rPenning, lambdaPenning, "ar");
  }
  fMediumMagboltz->SetTemperature(temperature);
  fMediumMagboltz->SetPressure(pressure); 
    fMediumMagboltz->EnableDebugging();
    fMediumMagboltz->Initialise(true);
  fMediumMagboltz->DisableDebugging();

  G4cout << gasFile << G4endl;
  G4AutoLock lock(&aMutex);
  if(gasName=="HeIso")
    fMediumMagboltz->LoadIonMobility(path + "/Data/IonMobility_He+_He.txt");
  else if(gasName=="ArCO2")
    fMediumMagboltz->LoadIonMobility(path + "/Data/IonMobility_Ar+_Ar.txt");
  
  if(gasFile!="")
      fMediumMagboltz->LoadGasFile(gasFile.c_str());
}
  

void HeedModel::buildBox(){
  geo = new Garfield::GeometrySimple();

  box = new Garfield::SolidTube(0., 0., 0.,0.,(detCon->GetGasBoxR())/CLHEP::cm,(detCon->GetGasBoxH()*0.5)/CLHEP::cm,0.,1.,0.);
  geo->AddSolid(box, fMediumMagboltz);
  
}

void HeedModel::BuildCompField(){
    // Switch between IROC and OROC.
    const bool iroc = false;
    // Switch gating on or off.
    bool gating = false;
    // y-axis gap between rows of wires [cm]
    const double gap = iroc ? 0.2 : 0.3;
    
    // y coordinates of the wires [cm]
    const double ys = gap;            // anode wires
    const double yc = 2. * gap;       // cathode
    const double yg = 2. * gap + 0.3; // gate
    // Periodicity (wire spacing)
    const double period = 0.25;
    const int nRep = 10;
    
    const double dc = period;
    const double dg = period / 2;
    
    // Wire diameters [cm]
    const double dSens = 0.0020;
    const double dCath = 0.0075;
    const double dGate = 0.0075;
    
    comp = new Garfield::ComponentAnalyticField();
    comp->SetGeometry(geo);
    
    
    
  
}

void HeedModel::BuildSensor(){
  fSensor = new Garfield::Sensor();
  //fSensor->SetTimeWindow(0.,fBinWidth,fNbins); //Lowest time [ns], time bins [ns], number of bins
}

void HeedModel::SetTracking(){
  if(driftRKF){
    fDriftRKF = new Garfield::DriftLineRKF();
    fDriftRKF->SetSensor(fSensor);
    fDriftRKF->EnableDebugging();
  }
  else{
    fAvalanche = new Garfield::AvalancheMicroscopic();
    fAvalanche->SetSensor(fSensor);
    fAvalanche->EnableSignalCalculation();  
    fDrift = new Garfield::AvalancheMC();
    fDrift->SetSensor(fSensor);
    fDrift->EnableSignalCalculation();
    fDrift->SetDistanceSteps(2.e-4);
    if(createAval) fDrift->EnableAttachment();
    else fDrift->DisableAttachment();
  }
  fTrackHeed = new Garfield::TrackHeed();
  fTrackHeed->SetSensor(fSensor);
  fTrackHeed->SetParticle("e-");
  fTrackHeed->EnableDeltaElectronTransport();

}
  
void HeedModel::CreateChamberView(){
  fChamber = new TCanvas("c", "Chamber View", 700, 700);
  cellView = new Garfield::ViewCell();
  cellView->SetComponent(comp);
  cellView->SetCanvas(fChamber);
  cellView->Plot2d();
  fChamber->Update();
  fChamber->Print("chamber_configuration.pdf");
//  gSystem->ProcessEvents();
  cout << "CreateCellView()" << endl;
  
  viewDrift = new Garfield::ViewDrift();
  viewDrift->SetCanvas(fChamber);
  fDrift->EnablePlotting(viewDrift);
  fTrackHeed->EnablePlotting(viewDrift);

}

void HeedModel::CreateSignalView(){
  fSignal = new TCanvas("c2", "Signal on the wire", 700, 700);
  viewSignal = new Garfield::ViewSignal();
  viewSignal->SetSensor(fSensor);
  viewSignal->SetCanvas(fSignal);

}

void HeedModel::CreateFieldView(){
  fField = new TCanvas("c3", "Weightingfield", 700, 700);
  viewField = new Garfield::ViewField();
  viewField->SetCanvas(fField);
  viewField->SetComponent(comp);
  viewField->SetArea(0.412,7.14,0.432,7.16);
  viewField->SetWeightingFieldRange(0.,500);
  viewField->SetNumberOfContours(40);
  viewField->PlotContourWeightingField("20","e");
  fField->Update();
  fField->Print("WeightingField_zoom_01mm.pdf");
}

void HeedModel::Drift(double x, double y, double z, double t){
    
}
