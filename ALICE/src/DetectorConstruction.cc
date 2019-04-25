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
#include "G4Cons.hh"
#include "G4IntersectionSolid.hh"
#include "G4Trd.hh"
#include "DetectorMessenger.hh"
#include "GasBoxSD.hh"
#include "SiliconSD.hh"
#include "HeedDeltaElectronModel.hh"
#include "HeedNewTrackModel.hh"
#include "G4SDManager.hh"


DetectorConstruction::DetectorConstruction(GasModelParameters* gmp)
    :
    fGasModelParameters(gmp),
    checkOverlaps(0),
    worldHalfLength(3.*m), //World volume is a cube with side length = 3m;
    wallThickness(0.05*m), //thickness of the aluminum walls
    caloThickness(1.*mm), // thickness of the silicon detectors
    gasPressure(1.*bar), // Pressure inside the gas
    temperature(273.15*kelvin), // temperature
    neonPercentage(85.72), // mixture settings
    co2Percentage(9.52)
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
  G4VisAttributes* purple = new G4VisAttributes(G4Colour(1.0, 0., 1.0));

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
  G4VPhysicalVolume* physiWorld = NULL;
  // TPC setup
  gasboxR = 0.25*m;
  gasboxH = 1.25*m;

  G4Element* elC = man->FindOrBuildElement("C");
  G4Element* elO = man->FindOrBuildElement("O");
  G4Element* elN = man->FindOrBuildElement("N");
  
  G4double molarMass = 20.17*g/mole;  // pure neon
  
  G4double gasDensityNe = nMoles * molarMass;
  G4cout << "gasPressure: " << G4BestUnit(gasPressure, "Pressure")
     << G4endl;
  G4cout << "gasDensityNe: " << G4BestUnit(gasDensityNe, "Volumic Mass")
     << G4endl;

  G4Material* neon = new G4Material("neon", 10, molarMass, gasDensityNe,
                                      kStateGas, temperature, gasPressure);
  G4double molfracNe = (neonPercentage/100.) * molarMass;

  // C4H10 Density 2.51 mg/mL (at 15 °C, 100 kPa=0.1 bar =100mBar)
  // GasDensity = 2.51*mg/cm3 ;
  molarMass = 44.01*g/mole;  // source wikipedia
  G4double gasDensityCO2 = nMoles * molarMass;
  G4cout << "gasDensityCO2: " << G4BestUnit(gasDensityCO2, "Volumic Mass")
         << G4endl;
  G4Material* CO2 = new G4Material("co2", gasDensityCO2, 2,
                                    kStateGas, temperature, gasPressure);
  CO2->AddElement(elC, 1);
  CO2->AddElement(elO, 2);
  
  G4double molfracCO2 = (co2Percentage/100.)*molarMass;
  
  molarMass = 28.0134*g/mole;  // source wikipedia
  G4double gasDensityN2 = nMoles * molarMass;
  G4cout << "gasDensityN2: " << G4BestUnit(gasDensityN2, "Volumic Mass")
  << G4endl;
  G4Material* N2 = new G4Material("n2",gasDensityN2,1,
                                   kStateGas, temperature, gasPressure);
  N2->AddElement(elN,1);
  G4double molfracN2 = (1-(neonPercentage+co2Percentage)/100.)*molarMass;

  G4double molfracNe_norm = molfracNe/(molfracNe+molfracCO2+molfracN2);
  G4double molfracCO2_norm = molfracCO2/(molfracNe+molfracCO2+molfracN2);
  G4double molfracN2_norm = 1-molfracNe_norm-molfracCO2_norm;

  G4cout << "Molar fraction Neon: " << molfracNe_norm << G4endl;
  G4cout << "Molar fraction CO2: " << molfracCO2_norm << G4endl;
  G4cout << "Molar fraction N2: " << molfracN2_norm << G4endl;

  G4double gasDensityMixture = (neonPercentage/100.) * gasDensityNe +
                               co2Percentage/100. * gasDensityCO2 + (1-(neonPercentage+co2Percentage)/100.)*gasDensityN2;
  
  mixture = new G4Material("mixture", gasDensityMixture, 3);
  
  
  mixture->AddMaterial(neon, molfracNe_norm);
  mixture->AddMaterial(CO2, molfracCO2_norm);
  mixture->AddMaterial(N2, molfracN2_norm);
  G4cout << "gasDensityC4H10He: " << G4BestUnit(gasDensityMixture,
                                                "Volumic Mass") << G4endl;
  
  //geometry dimensions:
  //Aluminum walls
  G4Material* aluminiumMaterial = man->FindOrBuildMaterial("G4_Al");
  
  //Silicon calorimeters
  G4Material* siliconMaterial =man->FindOrBuildMaterial("G4_Si");
  
  //World Volume
  G4Box* solidWorld = new G4Box("solidWorld_box", worldHalfLength, worldHalfLength, worldHalfLength);
  G4LogicalVolume* logicWorld =
  new G4LogicalVolume(solidWorld, vacuum, "solidWorld_log");
  
  physiWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld,
                                                    "solidWorld_phys", 0, false, 0, checkOverlaps);
  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
  
  //GasBox volume
  G4RotationMatrix* myRotation = new G4RotationMatrix();
  myRotation->rotateX(90.*deg);
  myRotation->rotateY(0.*deg);
  myRotation->rotateZ(0.*rad);
  G4Tubs* solidGasBox = new G4Tubs("solid_gasbox_tube",0,gasboxR,gasboxH*0.5, 0., twopi);
  logicGasBox =
  new G4LogicalVolume(solidGasBox, mixture, "solidGasBox_log");
  new G4PVPlacement(myRotation,G4ThreeVector(), logicGasBox,"solidGasBox_phys",logicWorld,false,0,checkOverlaps);
  
  //Silicon calorimeters
  G4Tubs* solidCalo = new G4Tubs("solid_tube_Calo",gasboxR,gasboxR+caloThickness,gasboxH*0.5, 0., twopi);
  logicCalo =
  new G4LogicalVolume(solidCalo, siliconMaterial, "solidCalo_log");
  new G4PVPlacement(myRotation,G4ThreeVector(), logicCalo,"solidCalo_phys",logicWorld,false,0,checkOverlaps);
  
  //Aluminum Walls
  G4Tubs* solidWalls = new G4Tubs("solid_tube_wall",0,gasboxR,wallThickness*0.5, 0., twopi);
  G4LogicalVolume* logicWall1 =
  new G4LogicalVolume(solidWalls, aluminiumMaterial, "solidWall1_log");
  G4LogicalVolume* logicWall2 =
  new G4LogicalVolume(solidWalls, aluminiumMaterial, "solidWall2_log");
  new G4PVPlacement(myRotation,G4ThreeVector(0,(gasboxH+wallThickness)/2.,0), logicWall1,
                    "solidWall1_phys",logicWorld,false,0,checkOverlaps);
  new G4PVPlacement(myRotation,G4ThreeVector(0,-(gasboxH+wallThickness)/2.,0), logicWall2,
                    "solidWall2_phys",logicWorld,false,0,checkOverlaps);

  //logicGasBox->SetVisAttributes(blue);
  logicWall1->SetVisAttributes(red);
  logicWall2->SetVisAttributes(red);
  //logicCalo->SetVisAttributes(yellow);
  
  
  //Construct a G4Region, connected to the logical volume in which you want to use the G4FastSimulationModel
  G4Region* regionGas = new G4Region("GasRegion");
  regionGas->AddRootLogicalVolume(logicGasBox);
    
  return physiWorld;

}

void DetectorConstruction::ConstructSDandField(){
  G4SDManager* SDManager = G4SDManager::GetSDMpointer();
  G4String GasBoxSDname = "interface/GasBoxSD";
  GasBoxSD* myGasBoxSD = new GasBoxSD(GasBoxSDname);
  SDManager->SetVerboseLevel(1);
  SDManager->AddNewDetector(myGasBoxSD);
  SetSensitiveDetector(logicGasBox,myGasBoxSD);

  G4String SiliconSDname = "interface/SiliconSD";
  SiliconSD* mySiliconSD = new SiliconSD(SiliconSDname);
  SDManager->SetVerboseLevel(1);
  SDManager->AddNewDetector(mySiliconSD);
  SetSensitiveDetector(logicCalo,mySiliconSD);

  //These commands generate the four gas models and connect it to the GasRegion
  G4Region* region = G4RegionStore::GetInstance()->GetRegion("GasRegion");
  new HeedNewTrackModel(fGasModelParameters,"HeedNewTrackModel",region,this,myGasBoxSD);
  new HeedDeltaElectronModel(fGasModelParameters,"HeedDeltaElectronModel",region,this,myGasBoxSD);
}

