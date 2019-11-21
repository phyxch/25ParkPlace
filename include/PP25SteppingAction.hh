// November 7, 2019: hexc and Zachary Grant
// First implementation of 25 Park Place building structure.

#ifndef PP25SteppingAction_h
#define PP25SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class PP25DetectorConstruction;
class PP25EventAction;

/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track 
/// lengths of charged particles in Absober and Gap layers and
/// updated in B4aEventAction.

class PP25SteppingAction : public G4UserSteppingAction
{
public:
  PP25SteppingAction(const PP25DetectorConstruction* detectorConstruction,
                    PP25EventAction* eventAction);
  virtual ~PP25SteppingAction();

  virtual void UserSteppingAction(const G4Step* step);
    
private:
  const PP25DetectorConstruction* fDetConstruction;
  PP25EventAction*  fEventAction;  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
