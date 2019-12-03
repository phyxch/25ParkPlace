// November 7, 2019: hexc and Zachary Grant
// First implementation of 25 Park Place building structure.
//
// November 19, 2019: hexc and Zachary Grant
// Add floor structure
//
// November 21, 2019: hexc and Zachary
// fine tuning the building floor structure
//
// December 3, 2019: hexc and Zachary
// Added telescope detector on each floor
//

#include "PP25DetectorConstruction.hh"

//#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4VisAttributes.hh"

#include "G4RegionStore.hh"
#include "G4GDMLParser.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "PP25FloorNumber.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* PP25DetectorConstruction::fMagFieldMessenger = nullptr; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PP25DetectorConstruction::PP25DetectorConstruction() : G4VUserDetectorConstruction()
{
    fCheckOverlaps = true;
    NbOfFloors = FLOOR_NUMBER;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PP25DetectorConstruction::~PP25DetectorConstruction()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* PP25DetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();
  
  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PP25DetectorConstruction::DefineMaterials()
{ 
  // Lead material defined using NIST Manager
  auto nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Pb");
 
  G4String symbol;  
 
  // Liquid argon material
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;  
  G4double density, fractionmass; 
  G4int ncomponents, natoms;

  G4Element* H  = new G4Element("Hydrogen",symbol="H" , z= 1., a= 1.01*g/mole);
  G4Element* C  = new G4Element("Carbon"  ,symbol="C" , z= 6., a= 12.01*g/mole);
  G4Element* N  = new G4Element("Nitrogen",symbol="N" , z= 7., a= 14.01*g/mole);
  G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z= 8., a= 16.00*g/mole);

  new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);
         // The argon by NIST Manager is a gas with a different density

                                                                                                                                         // Copied from pINT simulation
  scintMat = new G4Material("scintillator", density= 1.032*g/cm3, ncomponents=2);
  scintMat->AddElement(C, natoms=10);
  scintMat->AddElement(H, natoms=11);

 
  G4Material* Air = new G4Material("Air", density= 1.290*mg/cm3, ncomponents=2);
  Air->AddElement(N, fractionmass=0.7);
  Air->AddElement(O, fractionmass=0.3);

  floorMat = Air;

  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* PP25DetectorConstruction::DefineVolumes()
{
    // Define world volume
    worldSizeX  = 60.0*m;
    worldSizeY = 30*m;
    worldSizeZ  = 120.0*m; 
    
    // Geometry parameters
    nofLayers = 3;
    
    // Define scintillator dimension
    scintX  = scintY = 15.*cm;
    scintZ = 1.0*cm;

    dist12 = 10.0*cm;
    dist23 = 20.0*cm;

    // Define dimensions for the floor structure
    // The floor information was given to Zachary Grant by a construction project 
    // at GSU manager, Ronald W. Hixon, Jr., in November of 2019.
    //
    floorX = 56.54*m;
    floorY = 24.54*m;
    floorZ = 0.11*m;

    // Get materials
    auto defaultMaterial = G4Material::GetMaterial("Air");
    auto scintMaterial = G4Material::GetMaterial("scintillator");
    //    auto gapMaterial = G4Material::GetMaterial("liquidArgon");
    
    if ( ! defaultMaterial || ! scintMaterial) {
	G4ExceptionDescription msg;
	msg << "Cannot retrieve materials already defined."; 
	G4Exception("PP25DetectorConstruction::DefineVolumes()",
		    "MyCode0001", FatalException, msg);
    }  

    //     
    // World
    //
    auto worldS 
	= new G4Box("World",           // its name
		    worldSizeX/2, worldSizeY/2, worldSizeZ/2); // its size
    
    auto worldLV
	= new G4LogicalVolume(
			      worldS,           // its solid
			      defaultMaterial,  // its material
			      "World");         // its name
    
    auto physWorld
	= new G4PVPlacement(
			    0,                // no rotation
			    G4ThreeVector(),  // at (0,0,0)
			    worldLV,          // its logical volume                         
			    "World",          // its name
			    0,                // its mother  volume
			    false,            // no boolean operation
			    0,                // copy number
			    fCheckOverlaps);  // checking overlaps     

    // Contruct one floor structure
    // 
    
    
    // Visualization attributes
    //
    worldLV->SetVisAttributes (G4VisAttributes::GetInvisible());
    
    auto simpleBoxVisAtt= new G4VisAttributes(G4Colour(0.2,0.4,0.3));
    simpleBoxVisAtt->SetVisibility(true);
    simpleBoxVisAtt->SetForceWireframe(false);
    simpleBoxVisAtt->SetForceSolid(true);
    
    char Floor[8], Scint0[9], Scint1[9], Scint2[9];
    G4double floorHeight,  atmLayerHeight_prev;
    
    for (int iFloor = 0; iFloor < NbOfFloors; iFloor++) {
	sprintf(Floor, "Floor%02d", iFloor);
	floorHeight  = 0.5*115.2144*m - iFloor*(floorZ + 4.317*m);   // 4.317*m is the floor separation from Zachary
	floor_box.push_back(new G4Box(Floor, floorX/2.0, floorY/2.0, floorZ/2.0));
	floor_logic.push_back(new G4LogicalVolume(floor_box[iFloor], floorMat, Floor));
	floor_phys.push_back(new G4PVPlacement(0, G4ThreeVector(0, 0, floorHeight), Floor, floor_logic.at(iFloor), physWorld, false, 0));
	
	sprintf(Scint0, "Scint0%02d", iFloor);    // scintillator panel labeling:  ScintXYZ    X can be 0, 1, 2  and YZ can be 00, 01, .. 13.., for flooring
	scint0_box.push_back(new G4Box(Scint0, scintX/2.0, scintY/2.0, scintZ/2.0));
	scint0_logic.push_back(new G4LogicalVolume(scint0_box[iFloor], scintMat, Scint0));
	scint0_phys.push_back(new G4PVPlacement(0, G4ThreeVector(0, 0, floorHeight - 2.0*m), Scint0, scint0_logic.at(iFloor), physWorld, false, 0));

	sprintf(Scint1, "Scint1%02d", iFloor);
	scint1_box.push_back(new G4Box(Scint1, scintX/2.0, scintY/2.0, scintZ/2.0));
	scint1_logic.push_back(new G4LogicalVolume(scint1_box[iFloor], scintMat, Scint1));
	scint1_phys.push_back(new G4PVPlacement(0, G4ThreeVector(0, 0, floorHeight - 2.0*m-10*cm), Scint1, scint1_logic.at(iFloor), physWorld, false, 0));

	sprintf(Scint2, "Scint2%02d", iFloor);
	scint2_box.push_back(new G4Box(Scint2, scintX/2.0, scintY/2.0, scintZ/2.0));
	scint2_logic.push_back(new G4LogicalVolume(scint2_box[iFloor], scintMat, Scint2));
	scint2_phys.push_back(new G4PVPlacement(0, G4ThreeVector(0, 0, floorHeight - 2.0*m-30*cm), Scint2, scint2_logic.at(iFloor), physWorld, false, 0));

			     /*
	if ( iFloor == 6) 
	  {
	      //
	      // A small scintillator detector
	      //
	      
	      auto scintLayer0 
		  = new G4Box("scintLayer0",           // its name
			      scintX/2, scintY/2, scintZ/2); // its size
	      
	      auto scintLayerLV0
		  = new G4LogicalVolume(
					scintLayer0,                  // its solid
					scintMaterial,              // its material
					"scintLayerLV0");          // its name
	      
	      physScint0 = new G4PVPlacement(0,			                                                //no rotation
					     G4ThreeVector(0.0, 0.0, floorHeight+1.0*m),	//at (0,0,0)
					     "physScintLayer0",		//its name
					     scintLayerLV0,		//its logical volume				 
					     physWorld,			//its mother  volume
					     false,			//no boolean operation
					     0);			//copy number

	      auto scintLayer1
		  = new G4Box("scintLayer1",           // its name
			      scintX/2, scintY/2, scintZ/2); // its size
	      
	      auto scintLayerLV1
		  = new G4LogicalVolume(
					scintLayer1,                  // its solid
					scintMaterial,              // its material
					"scintLayerLV1");          // its name
	      
	      physScint1 = new G4PVPlacement(0,			                          //no rotation
					     G4ThreeVector(0.0, 0.0, floorHeight+1.0*m - 10*cm),	  //at (0,0,0)
					     "physScintLayer1",		//its name
					     scintLayerLV1,		//its logical volume				 
					     physWorld,			//its mother  volume
					     false,			//no boolean operation
					     0);			//copy number
	      
	      auto scintLayer2
		  = new G4Box("scintLayer2",           // its name
			      scintX/2, scintY/2, scintZ/2); // its size
	      
	      auto scintLayerLV2
		  = new G4LogicalVolume(
					scintLayer2,                  // its solid
					scintMaterial,              // its material
					"scintLayerLV2");          // its name
	      
	      physScint2 = new G4PVPlacement(0,			                                                //no rotation
					     G4ThreeVector(0.0, 0.0,  floorHeight + 1.0*m - 30*cm),	//at (0,0,0)
					     "physScintLayer2",		//its name
					     scintLayerLV2,		//its logical volume				 
					     physWorld,			//its mother  volume
					     false,			//no boolean operation
					     0);			//copy number
	      //
	      // Set the visulization property
	      scintLayerLV0->SetVisAttributes(simpleBoxVisAtt);
	      scintLayerLV1->SetVisAttributes(simpleBoxVisAtt);
	      scintLayerLV2->SetVisAttributes(simpleBoxVisAtt);
	      
	  }
			     */
  }

  
  //
  // Always return the physical World
  //
  return physWorld;
  
}

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
void PP25DetectorConstruction::ConstructSDandField()
{ 
  // Create global magnetic field messenger.
  // Uniform magnetic field is then created automatically if
  // the field value is not zero.
  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Register the field messenger for deleting
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
