// November 7, 2019: hexc and Zachary Grant
// First implementation of 25 Park Place building structure.

#include "PP25RunAction.hh"
#include "PP25Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PP25RunAction::PP25RunAction()
 : G4UserRunAction()
{ 
    // set printing event number per each event
    G4RunManager::GetRunManager()->SetPrintProgress(1);     
    
    // Create analysis manager
    // The choice of analysis technology is done via selectin of a namespace
    // in PP25Analysis.hh
    auto analysisManager = G4AnalysisManager::Instance();
    G4cout << "Using " << analysisManager->GetType() << G4endl;
    
    // Create directories 
    //analysisManager->SetHistoDirectoryName("histograms");
    //analysisManager->SetNtupleDirectoryName("ntuple");
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output
    
    // Book histograms, ntuple
    //
    
    // Creating histograms
    analysisManager->CreateH1("ElossLayer0","Edep in layer 0", 100, 0., 10*MeV);
    analysisManager->CreateH1("ElossLayer1","Edep in layer 1", 100, 0., 10*MeV);
    analysisManager->CreateH1("ElossLayer2","Edep in layer 2", 100, 0., 10*MeV);

    analysisManager->CreateH1("LengthLayer0","trackL in layer 0", 100, 0., 2*cm);    
    analysisManager->CreateH1("LengthLayer1","trackL in layer 1", 100, 0., 2*cm);
    analysisManager->CreateH1("LengthLayer2","trackL in layer 2", 100, 0., 2*cm);
    
    // Creating ntuple
    //
    analysisManager->CreateNtuple("PP25", "Edep and TrackL");
    analysisManager->CreateNtupleDColumn("ElossLayer0");
    analysisManager->CreateNtupleDColumn("ElossLayer1");
    analysisManager->CreateNtupleDColumn("ElossLayer2");
    analysisManager->CreateNtupleDColumn("LengthLayer0");
    analysisManager->CreateNtupleDColumn("LengthLayer1");
    analysisManager->CreateNtupleDColumn("LengthLayer2");
    analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PP25RunAction::~PP25RunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PP25RunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "PP25";
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PP25RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // print histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();
  if ( analysisManager->GetH1(1) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl; 
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl; 
    }
    
    for (int i = 0; i<3; i++)
	{
	    G4cout << " ElossLayer(" << i <<") : mean = " 
		   << G4BestUnit(analysisManager->GetH1(i)->mean(), "Energy") 
		   << " rms = " 
		   << G4BestUnit(analysisManager->GetH1(i)->rms(),  "Energy") << G4endl;
	    
	    G4cout << " LengthLayer(" << i <<") : mean = " 
		   << G4BestUnit(analysisManager->GetH1(i+3)->mean(), "Length") 
		   << " rms = " 
		   << G4BestUnit(analysisManager->GetH1(i+3)->rms(),  "Length") << G4endl;
	}	    
  }
  
  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
