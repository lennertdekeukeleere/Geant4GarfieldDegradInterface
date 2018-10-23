/*
 * HeedInterfaceModel.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: dpfeiffe
 */
#include <iostream>
#include "HeedInterfaceModel.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "HeedInterfaceMessenger.hh"

#include "G4SystemOfUnits.hh"

const static G4double torr = 1. / 760. * atmosphere;


HeedInterfaceModel::HeedInterfaceModel(G4String modelName, G4Region* envelope, DetectorConstruction* dc)
    : G4VFastSimulationModel(modelName, envelope),detCon(dc)	{
      fMapParticlesEnergy = new MapParticlesEnergy();
      fHeedInterfaceMessenger = new HeedInterfaceMessenger(this);
    }

HeedInterfaceModel::~HeedInterfaceModel() {}

G4bool HeedInterfaceModel::IsApplicable(const G4ParticleDefinition& particleType) {
  G4String particleName = particleType.GetParticleName();
  if (FindParticleName(particleName))
    return true;
  return false;
}

G4bool HeedInterfaceModel::ModelTrigger(const G4FastTrack& fastTrack) {
  G4double ekin = fastTrack.GetPrimaryTrack()->GetKineticEnergy();
  G4String particleName =
      fastTrack.GetPrimaryTrack()->GetParticleDefinition()->GetParticleName();
  if (FindParticleNameEnergy(particleName, ekin / keV))
		return true;
  return false;

}

void HeedInterfaceModel::DoIt(const G4FastTrack& fastTrack, G4FastStep& fastStep) {

  G4ThreeVector localdir = fastTrack.GetPrimaryTrackLocalDirection();

  G4ThreeVector worldPosition = fastTrack.GetPrimaryTrack()->GetPosition();

  G4double ekin = fastTrack.GetPrimaryTrack()->GetKineticEnergy();
  G4double time = fastTrack.GetPrimaryTrack()->GetGlobalTime();
  G4String particleName =
      fastTrack.GetPrimaryTrack()->GetParticleDefinition()->GetParticleName();

  fastStep.KillPrimaryTrack();
	fastStep.SetPrimaryTrackPathLength(0.0);
  RunHeed(particleName, ekin/keV, time, worldPosition.x() / CLHEP::cm,
      worldPosition.y() / CLHEP::cm, worldPosition.z() / CLHEP::cm,
      localdir.x(), localdir.y(), localdir.z());
	fastStep.SetTotalEnergyDeposited(ekin);
	edep+=ekin;

}

void HeedInterfaceModel::Initialise() {
  AddParticleName("e-", 1 * eV / keV, ELimit / keV);
  AddParticleName("gamma",1 * eV / keV, 1e+8 *MeV /keV);
}

void HeedInterfaceModel::AddParticleName(const G4String particleName,
                                      double ekin_min_keV,
                                      double ekin_max_keV) {
  if (ekin_min_keV >= ekin_max_keV) {
    return;
  }
  fMapParticlesEnergy->insert(
      std::make_pair(particleName, std::make_pair(ekin_min_keV, ekin_max_keV)));
  G4cout << "Particle added: " << ekin_min_keV << " " << ekin_max_keV << G4endl;    
}

G4bool HeedInterfaceModel::FindParticleName(G4String name) {
  MapParticlesEnergy::iterator it;
  it = fMapParticlesEnergy->find(name);
  if (it != fMapParticlesEnergy->end()) {
    return true;
  }
  return false;
}

G4bool HeedInterfaceModel::FindParticleNameEnergy(G4String name,
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

void HeedInterfaceModel::makeGas(){
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
  

void HeedInterfaceModel::buildBox(){
  geo = new Garfield::GeometrySimple();

  box = new Garfield::SolidBox(0., 0., 0.,(detCon->GetGasBoxR())/CLHEP::cm,(detCon->GetGasBoxH()*0.5)/CLHEP::cm,(detCon->GetGasBoxR())/CLHEP::cm);
  geo->AddSolid(box, fMediumMagboltz);
  
}

void HeedInterfaceModel::BuildCompField(){
  int cn = 0;
  comp = new Garfield::ComponentAnalyticField();
  comp->SetGeometry(geo);
  
  
}

void HeedInterfaceModel::BuildSensor(){
  fSensor = new Garfield::Sensor();

  if(transferAnalytical)
  fSensor->SetTransferFunction(transfer);
  else
  fSensor->SetTransferFunction(transferTimeVector,transferValuesVector);

  if(loadComsolfield) fSensor->AddComponent(voxfield);
  else fSensor->AddComponent(comp);

  int cn=0;
  
  for (int j = 0; j < detCon->GetNrUpperPlanes(); j++) {
    for (int i = 0; i < detCon->GetNrWiresPerPlane(); i++) {
    std::stringstream cn_stream;
    cn_stream << cn;
    fSensor->AddElectrode(comp,cn_stream.str());
    cn++;    
    }
  }

  for (int j = 0; j < detCon->GetNrLowerPlanes(); j++) {
    for (int i = 0; i < detCon->GetNrWiresPerPlane(); i++) {
      std::stringstream cn_stream;
    cn_stream << cn;
    fSensor->AddElectrode(comp,cn_stream.str());
    cn++;   
    }
  }
  fSensor->SetTimeWindow(0.,fBinWidth,fNbins); //Lowest time [ns], time bins [ns], number of bins
}

void HeedInterfaceModel::SetTracking(){
  if(driftRKF){
    fDriftRKF = new Garfield::DriftLineRKF();
    fDriftRKF->SetSensor(fSensor);
//    fDriftRKF->EnableDebugging();
  }
  else{
    fDrift = new Garfield::AvalancheMC();
    fDrift->SetSensor(fSensor);
    fDrift->EnableSignalCalculation();
  //    fDrift->EnableDebugging();
    if(createAval) fDrift->EnableAttachment();
    else fDrift->DisableAttachment();
    if(trackMicro){
      fAvalanche = new Garfield::AvalancheMicroscopic();
      fAvalanche->SetSensor(fSensor);
    }
  }
  fTrackHeed = new TrackHeedGeant();
    fTrackHeed->SetSensor(fSensor);
//  fTrackHeed->EnableDebugging();
  fTrackHeed->EnableDeltaElectronTransport();

}
  
void HeedInterfaceModel::CreateChamberView(){
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

void HeedInterfaceModel::CreateSignalView(){
  fSignal = new TCanvas("c2", "Signal on the wire", 700, 700);
  viewSignal = new Garfield::ViewSignal();
  viewSignal->SetSensor(fSensor);
  viewSignal->SetCanvas(fSignal);

}

void HeedInterfaceModel::CreateFieldView(){
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
