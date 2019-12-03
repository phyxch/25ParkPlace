// November 7, 2019: hexc and Zachary Grant
// First implementation of 25 Park Place building structure.

#include "PP25SteppingAction.hh"
#include "PP25EventAction.hh"
#include "PP25DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PP25SteppingAction::PP25SteppingAction(
                      const PP25DetectorConstruction* detectorConstruction,
                      PP25EventAction* eventAction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction),
    fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PP25SteppingAction::~PP25SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PP25SteppingAction::UserSteppingAction(const G4Step* step)
{
    // Collect energy and track length step by step
    
    // get volume of the current step
    auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
    
    // energy deposit
    auto edep = step->GetTotalEnergyDeposit();

    // to do
    // need to get the floor number
    //
    
    int iFloor;

    // step length
    G4double stepLength = 0.;
    if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
	stepLength = step->GetStepLength();
    }
    
    if ( volume == fDetConstruction->GetScintLayer0PV(iFloor) ) {
	fEventAction->AddEloss_TrackLength(0, edep,stepLength);
    } else if (volume == fDetConstruction->GetScintLayer1PV(iFloor)) {
	fEventAction->AddEloss_TrackLength(1, edep,stepLength);
    } else if (volume == fDetConstruction->GetScintLayer2PV(iFloor)) {
	fEventAction->AddEloss_TrackLength(2, edep,stepLength);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
