#include <iostream>
#include "HeedModel.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include <stdio.h>
#include "DriftLineTrajectory.hh"
#include "G4TrackingManager.hh"
#include "G4EventManager.hh"
#include "G4VVisManager.hh"

#include "G4AutoLock.hh"
namespace{G4Mutex aMutex = G4MUTEX_INITIALIZER;}

const static G4double torr = 1. / 760. * atmosphere;


HeedModel::HeedModel(G4String modelName, G4Region* envelope,DetectorConstruction* dc,GasBoxSD* sd)
: G4VFastSimulationModel(modelName, envelope), detCon(dc), fGasBoxSD(sd)	{}

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
  it = fMapParticlesEnergy.find(name);
  if (it != fMapParticlesEnergy.end()) {
    return true;
  }
  return false;
}

G4bool HeedModel::FindParticleNameEnergy(G4String name,
                                             double ekin_keV) {
  MapParticlesEnergy::iterator it;
//  it = fMapParticlesEnergy->find(name);
  for (it=fMapParticlesEnergy.begin(); it!=fMapParticlesEnergy.end();++it) {
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
  if(G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::workerRM){
    makeGas();
      
    buildBox();
    
    BuildCompField();
    
    BuildSensor();
    
    SetTracking();
    
    if(fVisualizeChamber) CreateChamberView();
    if(fVisualizeSignal) CreateSignalView();
    if(fVisualizeField) CreateFieldView();
  }
}

void HeedModel::makeGas(){
  fMediumMagboltz = new Garfield::MediumMagboltz();
  double pressure = detCon->GetGasPressure()/torr;
  double temperature = detCon->GetTemperature()/kelvin;
  double neonPerc = detCon->GetNeonPercentage();
  double co2Perc = detCon->GetCO2Percentage();
  double n2Perc = 1-neonPerc-co2Perc;
  G4String gasName = detCon->GetGasName();
  fMediumMagboltz->SetComposition("ne", neonPerc, "co2", co2Perc, "n2", n2Perc);
  fMediumMagboltz->SetTemperature(temperature);
  fMediumMagboltz->SetPressure(pressure); 
  fMediumMagboltz->EnableDebugging();
  fMediumMagboltz->Initialise(true);
  fMediumMagboltz->DisableDebugging();

  G4cout << gasFile << G4endl;
  const std::string path = getenv("GARFIELD_HOME");
  G4AutoLock lock(&aMutex);
  if(ionMobFile!="")
    fMediumMagboltz->LoadIonMobility(path + "/Data/" + ionMobFile);
  if(gasFile!="")
      fMediumMagboltz->LoadGasFile(gasFile.c_str());
}
  

void HeedModel::buildBox(){
  geo = new Garfield::GeometrySimple();

  box = new Garfield::SolidTube(0.,0., 0.,0.,(detCon->GetGasBoxR())/CLHEP::cm,(detCon->GetGasBoxH()*0.5)/CLHEP::cm,0.,1.,0.);
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
    const int nRep = 2;
    
    const double dc = period;
    const double dg = period / 2;
    
    // Wire diameters [cm]
    const double dSens = 0.0020;
    const double dCath = 0.0075;
    const double dGate = 0.0075;
    
    comp = new Garfield::ComponentAnalyticField();
    comp->SetGeometry(geo);
    
    comp->SetPeriodicityX(nRep * period);
    for (int i = 0; i < nRep; ++i) {
        comp->AddWire((i - 1) * period, (detCon->GetGasBoxH()*0.5)/CLHEP::cm - ys, dSens, vAnodeWires, "s");
    }
    for (int i = 0; i < nRep; ++i) {
        comp->AddWire(dc * (i - 0.5),(detCon->GetGasBoxH()*0.5)/CLHEP::cm - yc, dCath, vCathodeWires, "c");
    }
    for (int i = 0; i < nRep * 2; ++i) {
        const double xg = dg * (i - 1.5);
        comp->AddWire(xg,(detCon->GetGasBoxH()*0.5)/CLHEP::cm - yg, dGate, vGate, "g", 100., 50., 19.3, 1);
    }
    // Add the planes.
    comp->AddPlaneY((detCon->GetGasBoxH()*0.5)/CLHEP::cm, vPlaneLow, "pad_plane");
    comp->AddPlaneY(-(detCon->GetGasBoxH()*0.5)/CLHEP::cm, vPlaneHV, "HV");
    
    // Set the magnetic field [T].
    comp->SetMagneticField(0, 0.5, 0);
    
  
}

void HeedModel::BuildSensor(){
  fSensor = new Garfield::Sensor();
  fSensor->AddComponent(comp);
  //fSensor->SetTimeWindow(0.,fBinWidth,fNbins); //Lowest time [ns], time bins [ns], number of bins
}

void HeedModel::SetTracking(){
  if(driftRKF){
    fDriftRKF = new Garfield::DriftLineRKF();
    fDriftRKF->SetSensor(fSensor);
    fDriftRKF->EnableDebugging();
  }
  else if(trackMicro){
    fAvalanche = new Garfield::AvalancheMicroscopic();
    fAvalanche->SetSensor(fSensor);
    fAvalanche->EnableSignalCalculation();
  }
  else{  
    fDrift = new Garfield::AvalancheMC();
    fDrift->SetSensor(fSensor);
    fDrift->EnableSignalCalculation();
    fDrift->SetDistanceSteps(2.e-3);
    if(createAval) fDrift->EnableAttachment();
    else fDrift->DisableAttachment();
  }
  fTrackHeed = new Garfield::TrackHeed();
  fTrackHeed->SetSensor(fSensor);
  fTrackHeed->SetParticle("e-");
  fTrackHeed->EnableDeltaElectronTransport();

}
  
void HeedModel::CreateChamberView(){
  char str[30];
  strcpy(str,name);
  strcat(str,"_chamber");
  fChamber = new TCanvas(str, "Chamber View", 700, 700);
  cellView = new Garfield::ViewCell();
  cellView->SetComponent(comp);
  cellView->SetCanvas(fChamber);
  cellView->Plot2d();
  fChamber->Update();
  char str2[30];
  strcpy(str2,name);
  strcat(str2,"_chamber.pdf");
  fChamber->Print(str2);
//  gSystem->ProcessEvents();
  cout << "CreateCellView()" << endl;
  
  viewDrift = new Garfield::ViewDrift();
  viewDrift->SetCanvas(fChamber);
  if(driftRKF) fDriftRKF->EnablePlotting(viewDrift);
  else if(trackMicro) fAvalanche->EnablePlotting(viewDrift);
  else fDrift->EnablePlotting(viewDrift);
  fTrackHeed->EnablePlotting(viewDrift);

}

void HeedModel::CreateSignalView(){
  char str[30];
  strcpy(str,name);
  strcat(str,"_signal");
  fSignal = new TCanvas(str, "Signal on the wire", 700, 700);
  viewSignal = new Garfield::ViewSignal();
  viewSignal->SetSensor(fSensor);
  viewSignal->SetCanvas(fSignal);

}

void HeedModel::CreateFieldView(){
  char str[30];
  strcpy(str,name);
  strcat(str,"_efield");
  fField = new TCanvas(name, "Electric field", 700, 700);
  viewField = new Garfield::ViewField();
  viewField->SetCanvas(fField);
  viewField->SetComponent(comp);
  viewField->SetNumberOfContours(40);
  viewField->PlotContour();
  fField->Update();
  char str2[30];
  strcpy(str2,name);
  strcat(str2,"_efield.pdf");
  fField->Print(str2);
}

void HeedModel::Drift(double x, double y, double z, double t){
    if(driftElectrons){
        DriftLineTrajectory* dlt = new DriftLineTrajectory();
        G4TrackingManager* fpTrackingManager = G4EventManager::GetEventManager()->GetTrackingManager();
        fpTrackingManager->SetTrajectory(dlt);
        if(driftRKF){
            fDriftRKF->DriftElectron(x,y,z,t);
            unsigned int n = fDriftRKF->GetNumberOfDriftLinePoints();
            double xi,yi,zi,ti;
            for(int i=0;i<n;i++){
                fDriftRKF->GetDriftLinePoint(i,xi,yi,zi,ti);
                if(G4VVisManager::GetConcreteInstance() && i % 1000 == 0)
                  dlt->AppendStep(G4ThreeVector(xi*CLHEP::cm,yi*CLHEP::cm,zi*CLHEP::cm),ti);
            }
        }
        else if(trackMicro){
            fAvalanche->AvalancheElectron(x,y,z,t,0,0,0,0);
            unsigned int nLines = fAvalanche->GetNumberOfElectronEndpoints();
            for(int i=0;i<nLines;i++){
                unsigned int n = fAvalanche->GetNumberOfElectronDriftLinePoints(i);
                double xi,yi,zi,ti;
                for(int j=0;j<n;j++){
                    fAvalanche->GetElectronDriftLinePoint(xi,yi,zi,ti,j,i);
                    if(G4VVisManager::GetConcreteInstance() && i % 1000 == 0)
                      dlt->AppendStep(G4ThreeVector(xi*CLHEP::cm,yi*CLHEP::cm,zi*CLHEP::cm),ti);
                }
            }
        }
        else{
            fDrift->DriftElectron(x,y,z,t);
            unsigned int n = fDrift->GetNumberOfDriftLinePoints();
            double xi,yi,zi,ti;
            for(int i=0;i<n;i++){
                fDrift->GetDriftLinePoint(i,xi,yi,zi,ti);
                if(G4VVisManager::GetConcreteInstance() && i % 1000 == 0)
                  dlt->AppendStep(G4ThreeVector(xi*CLHEP::cm,yi*CLHEP::cm,zi*CLHEP::cm),ti);
            }
        }
    }
}

void HeedModel::PlotTrack(){
    if(fVisualizeChamber){
      G4cout << "PlotTrack" << G4endl;
      viewDrift->Plot(true,false);
      fChamber->Update();
      fChamber->Print("PrimaryTrack.pdf");
    }
}
