// November 7, 2019: hexc and Zachary Grant
// First implementation of 25 Park Place building structure.
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
    const G4VPhysicalVolume* GetScintLayer0PV() const;
    const G4VPhysicalVolume* GetScintLayer1PV() const;
    const G4VPhysicalVolume* GetScintLayer2PV() const;
     
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

    //std::vector<G4Material*> floor_Matt;
    std::vector<G4VPhysicalVolume*> floor_phys;
    std::vector<G4LogicalVolume*> floor_logic;
    std::vector<G4Box*> floor_box;
    
    G4double           worldSizeX, worldSizeY, worldSizeZ;
    G4double           dist12, dist23;
    G4double           scintX, scintY, scintZ;
    G4double           floorX, floorY, floorZ;  
    G4int                 nofLayers, nofFloors;

    G4VPhysicalVolume  *physScint0, *physScint1, *physScint2;
    G4Material  *floorMat;

};

// inline functions

inline const G4VPhysicalVolume* PP25DetectorConstruction::GetScintLayer0PV() const { 
  return physScint0; 
}

inline const G4VPhysicalVolume* PP25DetectorConstruction::GetScintLayer1PV() const { 
  return physScint1; 
}

inline const G4VPhysicalVolume* PP25DetectorConstruction::GetScintLayer2PV() const { 
  return physScint2; 
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

