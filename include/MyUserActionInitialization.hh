#ifndef MyUserActionInitialization_hh
#define MyUserActionInitialization_hh

#include "G4VUserActionInitialization.hh"

class DetectorConstruction;
class PhysicsList;

class MyUserActionInitialization : public G4VUserActionInitialization{
	public:
	MyUserActionInitialization(DetectorConstruction* dc, PhysicsList* phys);
	~MyUserActionInitialization();
	
	void Build() const;
	void BuildForMaster() const;
	
	private:
	DetectorConstruction* detCon;
	PhysicsList* physics;
	
};

#endif