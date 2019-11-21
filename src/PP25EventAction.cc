// November 7, 2019: hexc and Zachary Grant
// First implementation of 25 Park Place building structure.

#include "PP25EventAction.hh"
#include "PP25RunAction.hh"
#include "PP25Analysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PP25EventAction::PP25EventAction()
 : G4UserEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PP25EventAction::~PP25EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PP25EventAction::BeginOfEventAction(const G4Event* /*event*/)
{  
  // initialisation per event
    for (int j = 0;  j < 3; j++) 
	{
	    fEnergyLayer[j] = 0.;
	    fTrackLayer[j] = 0.;
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PP25EventAction::EndOfEventAction(const G4Event* event)
{
    // Accumulate statistics
    //
    
    // get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();
    
    // fill histograms
    for (int j = 0; j<3; j++) 
	{
	    analysisManager->FillH1(0+j, fEnergyLayer[j]);
	    analysisManager->FillH1(3+j, fTrackLayer[j]);
	}
    
    // fill ntuple
    analysisManager->FillNtupleDColumn(0, fEnergyLayer[0]);
    analysisManager->FillNtupleDColumn(1, fEnergyLayer[1]);
    analysisManager->FillNtupleDColumn(2, fEnergyLayer[2]);
    analysisManager->FillNtupleDColumn(3, fTrackLayer[0]);
    analysisManager->FillNtupleDColumn(4, fTrackLayer[1]);
    analysisManager->FillNtupleDColumn(5, fTrackLayer[2]);
    analysisManager->AddNtupleRow();  
    
    // Print per event (modulo n)
    //
    auto eventID = event->GetEventID();
    auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
    if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
	G4cout << "---> End of event: " << eventID << G4endl;     
	
	for (int j = 0; j<3; j++) 
	    {
		G4cout
		    << "   ELossLayer: total energy: " << std::setw(7)
		    << G4BestUnit(fEnergyLayer[j],"Energy")
		    << "       total track length: " << std::setw(7)
		    << G4BestUnit(fTrackLayer[j],"Length")
		    << G4endl;
	    }
    }
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
