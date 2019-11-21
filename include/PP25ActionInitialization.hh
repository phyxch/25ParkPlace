// November 7, 2019: hexc and Zachary Grant
// First implementation of 25 Park Place building structure.

#ifndef PP25ActionInitialization_h
#define PP25ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class PP25DetectorConstruction;

/// Action initialization class.
///

class PP25ActionInitialization : public G4VUserActionInitialization
{
  public:
    PP25ActionInitialization(PP25DetectorConstruction*);
    virtual ~PP25ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:
    PP25DetectorConstruction* fDetConstruction;
};

#endif

    
