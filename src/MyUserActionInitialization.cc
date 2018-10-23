#include "MyUserActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "G4SDManager.hh"
#include "GasBoxSD.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"
#include "MBTrackingAction.hh"

MyUserActionInitialization::MyUserActionInitialization(DetectorConstruction* dc, PhysicsList* phys):detCon(dc), physics(phys){}

MyUserActionInitialization::~MyUserActionInitialization(){
	G4cout << "Deleting MyUserActionInitialization" << G4endl;
}

void MyUserActionInitialization::Build() const {
	PrimaryGeneratorAction* primary = new PrimaryGeneratorAction(detCon->GetGunPosition());
	SetUserAction(primary);
	SteppingAction* stepAct = new SteppingAction(detCon);
	SetUserAction(stepAct);
	EventAction* evt = new EventAction(stepAct);
	SetUserAction(evt);
	SetUserAction(new RunAction(detCon,primary,physics,evt));
	if(detCon->GetFilterEvents()){
		G4cout << "Events will be filtered" << G4endl;
		SetUserAction(new StackingAction());
	}
//	SetUserAction(new MBTrackingAction());
	
}

void MyUserActionInitialization::BuildForMaster() const {
	PrimaryGeneratorAction* primary = new PrimaryGeneratorAction(detCon->GetGunPosition());
	EventAction* evt = new EventAction();
	SetUserAction(new RunAction(detCon,primary,physics,evt));
}
