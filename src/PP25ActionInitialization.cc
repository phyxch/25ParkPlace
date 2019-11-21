// November 7, 2019: hexc and Zachary Grant
// First implementation of 25 Park Place building structure.

#include "PP25ActionInitialization.hh"
#include "PP25PrimaryGeneratorAction.hh"
#include "PP25RunAction.hh"
#include "PP25EventAction.hh"
#include "PP25SteppingAction.hh"
#include "PP25DetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PP25ActionInitialization::PP25ActionInitialization
                            (PP25DetectorConstruction* detConstruction)
 : G4VUserActionInitialization(),
   fDetConstruction(detConstruction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PP25ActionInitialization::~PP25ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PP25ActionInitialization::BuildForMaster() const
{
  SetUserAction(new PP25RunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PP25ActionInitialization::Build() const
{
  SetUserAction(new PP25PrimaryGeneratorAction);
  SetUserAction(new PP25RunAction);
  auto eventAction = new PP25EventAction;
  SetUserAction(eventAction);
  SetUserAction(new PP25SteppingAction(fDetConstruction,eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
