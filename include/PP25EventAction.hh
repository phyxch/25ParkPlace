// November 7, 2019: hexc and Zachary Grant
// First implementation of 25 Park Place building structure.

#ifndef PP25EventAction_h
#define PP25EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#define nLayer 3          // Number of scintillator layers

/// Event action class
///

class PP25EventAction : public G4UserEventAction
{
  public:
    PP25EventAction();
    virtual ~PP25EventAction();

    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);
    
    void AddEloss_TrackLength(G4int n, G4double de, G4double dl);
    
  private:
    G4double  fEnergyLayer[nLayer];  
    G4double  fTrackLayer[nLayer]; 
};

// inline functions

inline void PP25EventAction::AddEloss_TrackLength(G4int layer, G4double de, G4double dl) {
  fEnergyLayer[layer] += de; 
  fTrackLayer[layer] += dl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
