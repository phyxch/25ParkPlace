// November 7, 2019: hexc and Zachary Grant
// First implementation of 25 Park Place building structure.

#ifndef PP25RunAction_h
#define PP25RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#define nLayer 3;

class G4Run;

/// Run action class
///
/// It accumulates statistic and computes dispersion of the energy deposit 
/// and track lengths of charged particles with use of analysis tools:
/// H1D histograms are created in BeginOfRunAction() for the following 
/// physics quantities:
/// - Edep in each layer
/// - Track length in each layer
///
/// The same values are also saved in the ntuple.
/// The histograms and ntuple are saved in the output file in a format
/// accoring to a selected technology in PP25Analysis.hh.
///
/// In EndOfRunAction(), the accumulated statistic and computed 
/// dispersion is printed.
///

class PP25RunAction : public G4UserRunAction
{
  public:
    PP25RunAction();
    virtual ~PP25RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

