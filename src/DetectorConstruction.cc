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
#include "HeedInterfaceModel.hh"
#include "HeedOnlyModel.hh"
#include "DegradModel.hh"
#include "G4SDManager.hh"


DetectorConstruction::DetectorConstruction()
    : checkOverlaps(0),
    worldHalfLength(3.*m), //World volume is a cube with side length = 3m;
    wallThickness(0.05*m), //thickness of the aluminum walls
    caloThickness(1.*mm), // thickness of the silicon detectors
    gasPressure(1.*bar),
    temperature(273.15*kelvin),
    setup("TPC"),
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
    
  if(setup == "TPC"){
      gasboxR = 0.25*m;
      gasboxH = 2.5*m;

      G4Element* elC = man->FindOrBuildElement("C");
      G4Element* elO = man->FindOrBuildElement("O");
      G4Element* elN = man->FindOrBuildElement("N");
      
      G4double molarMass = 20,17 * g / mole;  // pure neon
      
      G4double gasDensityNe = nMoles * molarMass;
      G4cout << "gasPressure: " << G4BestUnit(gasPressure, "Pressure")
         << G4endl;
      G4cout << "gasDensityNe: " << G4BestUnit(gasDensityNe, "Volumic Mass")
         << G4endl;

      G4Material* neon = new G4Material("neon", 2, molarMass, gasDensityNe,
                                          kStateGas, temperature, gasPressure);
      G4double molfracNe = (neonPercentage/100.) * molarMass;

      // C4H10 Density 2.51 mg/mL (at 15 °C, 100 kPa=0.1 bar =100mBar)
      // GasDensity = 2.51*mg/cm3 ;
      molarMass = 44.01  * g / mole;  // source wikipedia
      G4double gasDensityCO2 = nMoles * molarMass;
      G4cout << "gasDensityCO2: " << G4BestUnit(gasDensityCO2, "Volumic Mass")
             << G4endl;
      G4Material* CO2 = new G4Material("co2", 2, molarMass, gasDensityCO2,
                                        kStateGas, temperature, gasPressure);
      CO2->AddElement(elC, 1);
      CO2->AddElement(elO, 2);
      
      G4double molfracCO2 = (co2Percentage/100.)*molarMass;
      
      molarMass = 28.0134  * g / mole;  // source wikipedia
      G4double gasDensityN2 = nMoles * molarMass;
      G4cout << "gasDensityCO2: " << G4BestUnit(gasDensityN2, "Volumic Mass")
      << G4endl;
      G4Material* N2 = new G4Material("n2", 2, molarMass, gasDensityN2,
                                       kStateGas, temperature, gasPressure);
      N2->AddElement(elN, 2);
      G4double molfracN2 = ((1-co2Percentage-neonPercentage)/100.)*molarMass;

      molfracNe = molfracNe/(molfracNe+molfracCO2+molfracN2);
      molfracCO2 = molfracCO2/(molfracNe+molfracCO2+molfracN2);
      molfracN2 = 1-molfracNe-molfracCO2;

      G4double gasDensityMixture = (neonPercentage/100.) * gasDensityNe +
                                   co2Percentage/100. * gasDensityCO2 + ((1-neonPercentage-co2Percentage)/100.)*gasDensityN2;
      
      mixture = new G4Material("mixture", gasDensityMixture, 3);
      
      
      mixture->AddMaterial(neon, molfracNe);
      mixture->AddMaterial(CO2, molfracCO2);
      mixture->AddMaterial(N2, molfracN2);
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
      G4LogicalVolume* logicCalo =
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

      logicGasBox->SetVisAttributes(green);
      logicWall1->SetVisAttributes(red);
      logicWall2->SetVisAttributes(blue);
      logicCalo->SetVisAttributes(yellow);
      
      
  }
  else if(setup == "photo"){
      G4Material* air = man->FindOrBuildMaterial("G4_AIR");
      G4Material* lead = man->FindOrBuildMaterial("G4_Pb");
      G4Material* Xenon = man->FindOrBuildMaterial("Xenon900Torr");
      G4Material* glass= man->FindOrBuildMaterial("G4_MAGNESIUM_FLUORIDE");
      G4Material* kapton=man->FindOrBuildMaterial("G4_KAPTON");
      G4Material* steel304=man->FindOrBuildMaterial("StainlessSteel");
      G4Material* ceramic=man->FindOrBuildMaterial("ceramic");  //MACOR
      G4Material* CsIPhotocathode= man->FindOrBuildMaterial("G4_CESIUM_IODIDE");
      

      gasboxR = 3.5*cm;
      gasboxH = 25.*mm;

      G4double halfColimatorLength=0.2*cm;
      G4double rIntColimator=1.0*mm;
      G4double rIntColimator2=3.75*mm;
      G4double rExtColimator=5.*cm;
      G4double vesselExtRadius=gasboxR+0.5*cm;
      //detectorHalfZ=5*cm;
      G4double pmtHalfLength=0.5*cm;
      G4double photoCathodeHalfLength=10.*um;
      G4double pmtRadius=2.4*cm;
      G4double distColimattorToDetector=2.5*cm;
      G4double macorHalfY=1.3*cm/2;
      G4double windowHalfLength=25.*um;
      
      // geometries --------------------------------------------------------------
      // experimental hall (world volume)
      G4VSolid* worldSolid
      = new G4Box("worldBox",5.*cm,4.*cm,5.*cm);
      G4LogicalVolume* worldLogical  = new G4LogicalVolume(worldSolid,air,"worldLogical");
      physiWorld = new G4PVPlacement(0,G4ThreeVector(),worldLogical,"worldPhysical",0,
                          false,0,checkOverlaps);
      
      
      // colimator Tube
      G4VSolid* collimatorSolid = new G4Tubs("collimatorTube",rIntColimator,rExtColimator,halfColimatorLength,0.,twopi);
      G4LogicalVolume* collimatorLogical = new G4LogicalVolume(collimatorSolid,lead,"collimatorLogical");
      
      
      
      G4RotationMatrix rotm  = G4RotationMatrix();
      rotm.rotateX(90*deg);
      rotm.rotateY(0*deg);
      rotm.rotateZ(0*deg);
      
      G4ThreeVector position = G4ThreeVector(0.,-halfColimatorLength-distColimattorToDetector,0.*mm);
      
      G4Transform3D transform = G4Transform3D(rotm,position);
      
      
      
      G4VPhysicalVolume* collimatorPhysical = new G4PVPlacement(transform,collimatorLogical,"collimatorPhysical",worldLogical,
                                                                false,0,checkOverlaps);
      
      
      
      
      
      // colimator Window
      G4VSolid* collimatorSolid2 = new G4Tubs("collimatorWindow",rIntColimator2,rExtColimator,halfColimatorLength,0.,twopi);
      G4LogicalVolume* collimatorLogical2 = new G4LogicalVolume(collimatorSolid2,steel304,"collimatorLogical2");
      
      
      G4ThreeVector position2 = G4ThreeVector(0.,-halfColimatorLength,0.*mm);
      
      G4Transform3D transform2 = G4Transform3D(rotm,position2);
      
      
      
      G4VPhysicalVolume* collimatorPhysical2 = new G4PVPlacement(transform2,collimatorLogical2,"collimatorPhysical2",worldLogical,
                                                                 false,0,checkOverlaps);
      
      
      
      
      //Radiation window in kapton
      
      
      // colimator Window
      G4VSolid* window = new G4Tubs("Window",0,rIntColimator2,windowHalfLength,0.,twopi);
      
      G4LogicalVolume* windowLogical = new G4LogicalVolume(window,kapton,"windowLogical");
      
      
      G4ThreeVector position5 = G4ThreeVector(0.,-windowHalfLength,0.*mm);
      
      G4Transform3D transform5 = G4Transform3D(rotm,position5);
      
      
      
      G4VPhysicalVolume* windowPhysical = new G4PVPlacement(transform5,windowLogical,"windowPhysical",worldLogical,
                                                            false,0,checkOverlaps);
      
      
      
      
      
      //Make XenonVolume
      
      
      
      G4VSolid* detectorSolid=new  G4Tubs("detectorBox",0,gasboxR,gasboxH*0.5,0.,twopi);
      
      ////Place Detector in world
      logicGasBox = new G4LogicalVolume(detectorSolid,Xenon,"detectorLogical");
      
      G4ThreeVector position3 = G4ThreeVector(0.,gasboxH*0.5,0.*mm);
      G4Transform3D transform3 = G4Transform3D(rotm,position3);
      
      G4VPhysicalVolume* detectorPhysical=    new G4PVPlacement(transform3,             //rotation and position
                                                                logicGasBox,            //its logical volume
                                                                "detectorPhysical",             //its name
                                                                worldLogical,             //its mother  volume
                                                                true,                 //no boolean operation
                                                                0,                 //copy number
                                                                checkOverlaps);       // checking overlaps
      
      
      //Macor
      
      
      G4VSolid* macorSolid=new  G4Tubs("macorTube",gasboxR,rExtColimator,macorHalfY,0.,twopi);
      
      ////Place macor in world
      G4LogicalVolume* macorLogical = new G4LogicalVolume(macorSolid,ceramic,"macorLogical");
      
      G4ThreeVector position4 = G4ThreeVector(0.,macorHalfY,0.*mm);
      G4Transform3D transform4 = G4Transform3D(rotm,position4);
      
      G4VPhysicalVolume* macorPhysical=    new G4PVPlacement(transform4,             //rotation and position
                                                             macorLogical,            //its logical volume
                                                             "macorPhysical",             //its name
                                                             worldLogical,             //its mother  volume
                                                             true,                 //no boolean operation
                                                             0,                 //copy number
                                                             checkOverlaps);       // checking overlaps
      
      //PMT
      G4VSolid* pmtSolid = new G4Tubs("pmtWindow",0.,pmtRadius,pmtHalfLength,0.,twopi);
      G4LogicalVolume* pmtLogical = new G4LogicalVolume(pmtSolid,glass,"pmtLogical");
      
      
      //MAke pmtLogical mother and photocathode daugther
      G4ThreeVector positionPMT = G4ThreeVector(0.,pmtHalfLength+2*gasboxH*0.5,0.*mm);
      
      
      G4Transform3D transformPMT = G4Transform3D(rotm,positionPMT);
      
      
      
      G4VPhysicalVolume* pmtPhysical= new G4PVPlacement(transformPMT,pmtLogical,"pmtPhysical",worldLogical,
                                                        false,0,checkOverlaps);
      
      
      //OPTICAL SURFACES
      
      //Xenon-GLASS
      
      G4OpticalSurface* opXenon_Glass = new G4OpticalSurface("XenonGlassSurface");
      opXenon_Glass->SetModel(unified);                  // SetModel
      opXenon_Glass->SetType(dielectric_dielectric);   // SetType
      opXenon_Glass->SetFinish(polished);                 // SetFinish
      
      G4LogicalBorderSurface* XenonGlass = new G4LogicalBorderSurface("XenonGlass",detectorPhysical,pmtPhysical,opXenon_Glass);
      
      //    // visualization attributes ------------------------------------------------
      
      worldLogical->->SetVisAttributes(G4VisAttributes::Invisible);
      collimatorLogical->SetVisAttributes(red);
      collimatorLogical2->SetVisAttributes(green);
      logicGasBox->SetVisAttributes(blue);
      pmtLogical->SetVisAttributes(yellow);
      windowLogical->SetVisAttributes(purple);
  }


  //Construct a G4Region, connected to the logical volume in which you want to use the G4FastSimulationModel
  G4Region* regionGas = new G4Region("GasRegion");
  regionGas->AddRootLogicalVolume(logicGasBox);
    
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

