#include "DetectorConstruction.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4RotationMatrix.hh"
#include "G4UnitsTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Trd.hh"
#include "G4Threading.hh"
#include "G4RegionStore.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "CADMesh.hh"
#include "G4Cons.hh"
#include "G4IntersectionSolid.hh"
#include "G4Trd.hh"
#include "DetectorMessenger.hh"
#include "GasBoxSD.hh"
#include "HeedInterfaceModel.hh"
#include "HeedOnlyModel.hh"
#include "DegradModel.hh"
#include "G4SDManager.hh"

DetectorConstruction::DetectorConstruction()
    : checkOverlaps(0),
    worldHalfLength(3.*m), //World volume is a cube with side length = 3m;
    gasboxR(0.25*m), // radius of tube filled with gas
    gasboxH (2.5*m), // length of the tube
    wallThickness(0.05*m), //thickness of the aluminum walls
    caloThickness(1.*mm), // thickness of the silicon detectors
    gasPressure(0.6*bar),
    temperature(273.15*kelvin),
    gasName("HeIso")
{
  detectorMessenger = new DetectorMessenger(this);


}

DetectorConstruction::~DetectorConstruction() {
  delete detectorMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct(){
  /* The World volume is a vacuum in which a gastube is placed with the walls made out of Aluminum. The
  endcaps are Silicon detectors, used as calorimeter 
  */
    
  //Colors for visualization
  G4VisAttributes* red = new G4VisAttributes(G4Colour(1., 0., 0.));
  G4VisAttributes* green = new G4VisAttributes(G4Colour(0., 1., 0.));
  G4VisAttributes* blue = new G4VisAttributes(G4Colour(0., 0., 1.));
  G4VisAttributes* yellow = new G4VisAttributes(G4Colour(1.0, 1.0, 0.));

  /*First: build materials
    World: vacuum
    Walls: Aluminum
    Gas: mixture of Helium and Isobutane or Ar and CO2
    Calorimeter: Silicon 
  */
  
    
  //World material: vacuum
  G4NistManager* man = G4NistManager::Instance();
  man->SetVerbose(1);
  G4Material* vacuum = man->FindOrBuildMaterial("G4_Galactic");
  
  //Gas material: mixture of HeIso or ArCO2
  G4double nMoles = gasPressure / (8.314 * joule / mole * temperature);
  G4Material* mixture=NULL;
  if(gasName == "HeIso"){

      G4double molarMass = 4.003 * g / mole;  // pure helium
      
      G4double gasDensityHe = nMoles * molarMass;
      G4cout << "gasPressure: " << G4BestUnit(gasPressure, "Pressure")
         << G4endl;
      G4cout << "gasDensityHe: " << G4BestUnit(gasDensityHe, "Volumic Mass")
         << G4endl;

      G4Material* helium = new G4Material("helium", 2, molarMass, gasDensityHe,
                                          kStateGas, temperature, gasPressure);
      G4double molfracHe = (1.0 - addmixturePercentage/100.) * molarMass;

      // C4H10 Density 2.51 mg/mL (at 15 °C, 100 kPa=0.1 bar =100mBar)
      // GasDensity = 2.51*mg/cm3 ;
      molarMass = 58.12 * g / mole;  // source wikipedia
      G4double gasDensityC4H10 = nMoles * molarMass;
      G4cout << "gasDensityC4H10: " << G4BestUnit(gasDensityC4H10, "Volumic Mass")
             << G4endl;
      
      G4double molfracIso = (addmixturePercentage/100.)*molarMass;

      molfracHe = molfracHe/(molfracHe+molfracIso);
      molfracIso = 1-molfracHe;

      G4Material* C4H10 = new G4Material("C4H10", gasDensityC4H10, 2);
      G4Element* elH = man->FindOrBuildElement(1);
      G4Element* elC = man->FindOrBuildElement(6);
      C4H10->AddElement(elC, 4);
      C4H10->AddElement(elH, 10);

      G4double gasDensityMixture = (1.0 - addmixturePercentage/100.) * gasDensityHe +
                                   addmixturePercentage/100. * gasDensityC4H10;

      mixture = new G4Material("mixture", gasDensityMixture, 2);
      
      
      mixture->AddMaterial(C4H10, molfracIso);
      mixture->AddMaterial(helium, molfracHe);
      G4cout << "gasDensityC4H10He: " << G4BestUnit(gasDensityMixture,
                                                    "Volumic Mass") << G4endl;
      
  }
  else if(gasName == "ArCO2"){
      G4double molarMass = 39.948 * g / mole;  // pure helium
      G4double gasDensityAr = nMoles * molarMass;
      G4cout << "gasPressure: " << G4BestUnit(gasPressure, "Pressure")
         << G4endl;
      G4cout << "gasDensityHe: " << G4BestUnit(gasDensityAr, "Volumic Mass")
         << G4endl;

      G4Material* argon = new G4Material("argon", 18, molarMass, gasDensityAr,
                                          kStateGas, temperature, gasPressure);
      G4double molfracAr = (1.0 - addmixturePercentage/100.) * molarMass;

      // C4H10 Density 2.51 mg/mL (at 15 °C, 100 kPa=0.1 bar =100mBar)
      // GasDensity = 2.51*mg/cm3 ;
      molarMass = 44.01 * g / mole;  // source wikipedia
      G4double gasDensityCO2 = nMoles * molarMass;
      G4cout << "gasDensityC4H10: " << G4BestUnit(gasDensityCO2, "Volumic Mass")
             << G4endl;
      
      G4double molfracCO2 = (addmixturePercentage/100.)*molarMass;

      molfracAr = molfracAr/(molfracAr+molfracCO2);
      molfracCO2 = 1-molfracAr;

      G4Material* CO2 = new G4Material("CO2", gasDensityCO2, 2);
      G4Element* elC = man->FindOrBuildElement(6);
      G4Element* elO = man->FindOrBuildElement("O");
      CO2->AddElement(elC, 1);
      CO2->AddElement(elO, 2);

      G4double gasDensityMixture = (1.0 - addmixturePercentage/100.) * gasDensityAr +
                                   addmixturePercentage/100. * gasDensityCO2;

      mixture = new G4Material("mixture", gasDensityMixture, 2);
      
      
      mixture->AddMaterial(CO2, molfracCO2);
      mixture->AddMaterial(argon, molfracAr);
      G4cout << "gasDensityC4H10He: " << G4BestUnit(gasDensityMixture,
                                                    "Volumic Mass") << G4endl;
  }

  //geometry dimensions:
  //Aluminum walls
  G4Material* aluminiumMaterial = man->FindOrBuildMaterial("G4_Al");
  
  //Silicon calorimeters
  G4Material* siliconMaterial =man->FindOrBuildMaterial("G4_Si");
  
  //World Volume
  G4Box* solidWorld = new G4Box("solidWorld_box", worldHalfLength, worldHalfLength, worldHalfLength);
  G4LogicalVolume* logicWorld =
      new G4LogicalVolume(solidWorld, vacuum, "solidWorld_log");
  
  G4VPhysicalVolume* physiWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld,
                               "solidWorld_phys", 0, false, 0, checkOverlaps);
  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
  
  //GasBox volume
  G4RotationMatrix* myRotation = new G4RotationMatrix();
  myRotation->rotateX(90.*deg);
  myRotation->rotateY(0.*deg);
  myRotation->rotateZ(0.*rad);
  G4Tubs* solidGasBox = new G4Tubs("solid_gasbox_tube",0,gasboxR,gasboxH, 0., twopi);
  logicGasBox =
      new G4LogicalVolume(solidGasBox, mixture, "solidGasBox_log");
  new G4PVPlacement(myRotation,G4ThreeVector(), logicGasBox,"solidGasBox_phys",logicWorld,false,0,checkOverlaps);

  //Silicon calorimeters
  G4Tubs* solidCalo = new G4Tubs("solid_tube_Calo",gasboxR,gasboxR+caloThickness,1.*m, 0., twopi);
  G4LogicalVolume* logicCalo =
      new G4LogicalVolume(solidCalo, siliconMaterial, "solidCalo_log");
  new G4PVPlacement(myRotation,G4ThreeVector(), logicCalo,"solidCalo_phys",logicWorld,false,0,checkOverlaps);

  //Aluminum Walls
  G4Tubs* solidWalls = new G4Tubs("solid_tube_wall",0,gasboxR,wallThickness, 0., twopi);
  G4LogicalVolume* logicWall1 =
      new G4LogicalVolume(solidWalls, aluminiumMaterial, "solidWall1_log");
  G4LogicalVolume* logicWall2 =
      new G4LogicalVolume(solidWalls, aluminiumMaterial, "solidWall2_log");
  new G4PVPlacement(myRotation,G4ThreeVector(0,(gasboxH+wallThickness)/2.,0), logicWall1,
    "solidCalo1_phys",logicWorld,false,0,checkOverlaps);
  new G4PVPlacement(myRotation,G4ThreeVector(0,-(gasboxH+wallThickness)/2.,0), logicWall2,
    "solidCalo2_phys",logicWorld,false,0,checkOverlaps);


  //Construct a G4Region, connected to the logical volume in which you want to use the G4FastSimulationModel
  G4Region* regionGas = new G4Region("GasRegion");
  regionGas->AddRootLogicalVolume(logicGasBox);

  logicGasBox->SetVisAttributes(green);
  logicWall1->SetVisAttributes(red);
  logicWall2->SetVisAttributes(blue);
  logicCalo->SetVisAttributes(yellow);
    
  return physiWorld;

}

void DetectorConstruction::ConstructSDandField(){
  G4Region* region = G4RegionStore::GetInstance()->GetRegion("GasRegion");
  HeedOnlyModel* HOM = new HeedOnlyModel(fGasModelParameters,"HeedOnlyModel",region,this);
  HeedInterfaceModel* HIM = new HeedInterfaceModel(fGasModelParameters,"HeedInterfaceModel",region,this);
  DegradModel* DM = new DegradModel(fGasModelParameters,"DegradModel",region,this);

  G4SDManager* SDManager = G4SDManager::GetSDMpointer();
  G4String GasBoxSDname = "gasbox/myGasBoxSD";
  GasBoxSD* myGasBoxSD = new GasBoxSD(GasBoxSDname);
  SDManager->SetVerboseLevel(1);
  SDManager->AddNewDetector(myGasBoxSD);
  SetSensitiveDetector(logicGasBox,myGasBoxSD);

}

