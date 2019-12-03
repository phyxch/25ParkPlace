// November 7, 2019: hexc and Zachary Grant
// First implementation of 25 Park Place building structure.
// 
// December 3, 2019: hexc and Zachary
// Added telescope detector on each floor
//

#ifndef PP25DetectorConstruction_h
#define PP25DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Box.hh"
#include "G4Material.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;

class PP25DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    PP25DetectorConstruction();
    virtual ~PP25DetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    // get methods
    //
    const G4VPhysicalVolume* GetScintLayer0PV(int) const;
    const G4VPhysicalVolume* GetScintLayer1PV(int) const;
    const G4VPhysicalVolume* GetScintLayer2PV(int) const;
     
  private:
    // methods
    //
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
  
    // data members
    //
    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger; 
                                      // magnetic field messenger
     
    //    G4VPhysicalVolume*   fAbsorberPV; // the absorber physical volume
    
    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps

    G4int NbOfFloors;

    // Floor structure
    //std::vector<G4Material*> floor_Matt;
    std::vector<G4VPhysicalVolume*> floor_phys;
    std::vector<G4LogicalVolume*> floor_logic;
    std::vector<G4Box*> floor_box;

    // telescope array
    std::vector<G4VPhysicalVolume*> scint0_phys, scint1_phys, scint2_phys;
    std::vector<G4LogicalVolume*> scint0_logic, scint1_logic, scint2_logic;
    std::vector<G4Box*> scint0_box, scint1_box, scint2_box;

    
    G4double           worldSizeX, worldSizeY, worldSizeZ;
    G4double           dist12, dist23;
    G4double           scintX, scintY, scintZ;
    G4double           floorX, floorY, floorZ;  
    G4int                 nofLayers, nofFloors;

    //    G4VPhysicalVolume  *physScint0, *physScint1, *physScint2;
    G4Material *floorMat, *scintMat;

};

// inline functions

inline const G4VPhysicalVolume* PP25DetectorConstruction::GetScintLayer0PV(int iFloor) const { 
    return scint0_phys.at(iFloor); 
}

inline const G4VPhysicalVolume* PP25DetectorConstruction::GetScintLayer1PV(int iFloor) const { 
    return scint1_phys.at(iFloor); 
}

inline const G4VPhysicalVolume* PP25DetectorConstruction::GetScintLayer2PV(int iFloor) const { 
    return scint2_phys.at(iFloor); 
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

