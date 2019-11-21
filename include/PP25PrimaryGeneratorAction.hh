// November 7, 2019: hexc and Zachary Grant
// First implementation of 25 Park Place building structure.

#ifndef PP25PrimaryGeneratorAction_h
#define PP25PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

/// The primary generator action class with particle gum.
///
/// It defines a single particle which hits the calorimeter 
/// perpendicular to the input face. The type of the particle
/// can be changed via the G4 build-in commands of G4ParticleGun class 
/// (see the macros provided with this example).

class PP25PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PP25PrimaryGeneratorAction();    
    virtual ~PP25PrimaryGeneratorAction();
    
    virtual void GeneratePrimaries(G4Event* event);
    
    // set methods
    void SetRandomFlag(G4bool value);
    
private:
    G4ParticleGun*  fParticleGun; // G4 particle gun
    G4int ev_type, evtID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
