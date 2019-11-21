// November 7, 2019: hexc and Zachary Grant
// First implementation of 25 Park Place building structure.
//
// November 21, 2019: hexc and Zachary
// Define the proper incoming cosmic ray source

#include "PP25PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PP25PrimaryGeneratorAction::PP25PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(nullptr)
{
    // set event type
    ev_type = 1;
    
    G4int nofParticles = 1;
    fParticleGun = new G4ParticleGun(nofParticles);
    
    // default particle kinematic
    //
    auto particleDefinition 
	= G4ParticleTable::GetParticleTable()->FindParticle("e-");
    fParticleGun->SetParticleDefinition(particleDefinition);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,1.,0.));
    fParticleGun->SetParticleEnergy(50.*MeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PP25PrimaryGeneratorAction::~PP25PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PP25PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // This function is called at the begining of event
    
    // In order to avoid dependence of PrimaryGeneratorAction
    // on DetectorConstruction class we get world volume 
    // from G4LogicalVolumeStore
    //
    // This function is called at the begining of event
    G4ParticleDefinition* particle;
    
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

    G4double worldZHalfLength = 0.;
    auto worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
    
    // Check that the world volume has box shape
    G4Box* worldBox = nullptr;
    if (  worldLV ) {
	worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
    }
    
    if ( worldBox ) {
	worldZHalfLength = worldBox->GetZHalfLength();  
    }
    else  {
	G4ExceptionDescription msg;
	msg << "World volume of box shape not found." << G4endl;
	msg << "Perhaps you have changed geometry." << G4endl;
	msg << "The gun will be place in the center.";
	G4Exception("PP25PrimaryGeneratorAction::GeneratePrimaries()",
		    "MyCode0002", JustWarning, msg);
    } 
    
    
    G4double xPos, yPos, zPos, xVec, yVec, zVec, sigmaAngle, theta, phi, momentum, sigmaMomentum, mass, pp, Ekin;
    G4int pid;
    
    // Set up cosmic ray events
    if (ev_type == 0) {        // single particle simulation
	pid = 0;   // tmp pID
	//	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,-1.,0.));
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
	//   fParticleGun->SetParticleEnergy(1.*GeV);
	yPos = 2*(G4UniformRand()-0.5)*(worldBox->GetYHalfLength());
	xPos = 2*(G4UniformRand()-0.5)*(worldBox->GetXHalfLength());
	zPos = worldZHalfLength + 5.0*m;
	fParticleGun->SetParticlePosition(G4ThreeVector(xPos, yPos, zPos)); 
	
	sigmaAngle = 140.*deg;
	theta = (G4UniformRand()-0.5)*sigmaAngle;
	phi = G4UniformRand()*360.*deg;
	xVec = std::cos(phi)*std::sin(theta);
	yVec = std::sin(phi)*std::sin(theta);
	zVec = -std::cos(theta);
	fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xVec, yVec, zVec));
	
	fParticleGun->GeneratePrimaryVertex(anEvent);
	
    }  else if (ev_type == 1) {    // cosmic ray muon flux event
	pid = 1;   // tmp pID
	particle = particleTable->FindParticle("mu-");
	fParticleGun->SetParticleDefinition(particle);
	
	//fParticleGun->SetParticleTime(0.0*ns);
	
	momentum = 300.*MeV;
	sigmaMomentum = 10000.*MeV;
	
	mass = particle->GetPDGMass();
	
	sigmaAngle = 140.*deg;
	
	G4int npart = 1;
	for (G4int i=1; i<=npart; i++) {
	    pp = momentum + G4UniformRand()*sigmaMomentum;  // 0 GeV < pp << 10.3 GeV    
	    Ekin = std::sqrt(pp*pp+mass*mass) - mass;
	    fParticleGun->SetParticleEnergy(Ekin);
	    yPos = 2*(G4UniformRand()-0.5)*(worldBox->GetYHalfLength());
	    xPos = 2*(G4UniformRand()-0.5)*(worldBox->GetXHalfLength());
	    zPos = worldZHalfLength + 5.0*m;
	    //	    yPos = (G4UniformRand()-0.5)*30.0*cm;
	    //	    zPos = (G4UniformRand()-0.5)*30.0*cm;
	    //	    xPos = 25.0*cm;
	    fParticleGun->SetParticlePosition(G4ThreeVector(xPos, yPos, zPos)); 
	    
	    theta = (G4UniformRand()-0.5)*sigmaAngle;
	    phi = G4UniformRand()*360.*deg;
	    xVec = std::cos(phi)*std::sin(theta);
	    yVec = std::sin(phi)*std::sin(theta);
	    zVec = -std::cos(theta);
	    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xVec, yVec, zVec));
	    
	    fParticleGun->GeneratePrimaryVertex(anEvent);
	}
    } else if (ev_type == 2)  // cosmic ray neutron event
	{
	    pid = 2;   // tmp pID
	    particle = particleTable->FindParticle("neutron");
	    fParticleGun->SetParticleDefinition(particle);
	    
	    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
	    
	    momentum = 1.*MeV;
	    sigmaMomentum = 100.*MeV;
	    
	    mass = particle->GetPDGMass();
	    
	    sigmaAngle = 140.*deg;
	    
	    G4int npart = 1;
	    for (G4int i=1; i<=npart; i++) {
		pp = momentum + G4UniformRand()*sigmaMomentum;  // 0 GeV < pp << 10.3 GeV    
		Ekin = std::sqrt(pp*pp+mass*mass) - mass;
		fParticleGun->SetParticleEnergy(Ekin);
		yPos = 2*(G4UniformRand()-0.5)*(worldBox->GetYHalfLength());
		xPos = 2*(G4UniformRand()-0.5)*(worldBox->GetXHalfLength());
		zPos = worldZHalfLength + 5.0*m;
		fParticleGun->SetParticlePosition(G4ThreeVector(xPos, yPos, zPos)); 
		
		theta = (G4UniformRand()-0.5)*sigmaAngle;
		phi = G4UniformRand()*360.*deg;
		xVec = std::cos(phi)*std::sin(theta);
		yVec = std::sin(phi)*std::sin(theta);
		zVec = -std::cos(theta);
		fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xVec, yVec, zVec));
		
		fParticleGun->GeneratePrimaryVertex(anEvent);
	    }
	}else if (ev_type == 3)  // single particle simulation
	{
	    pid = 3;   // tmp pID
	    particle = particleTable->FindParticle("mu-");
	    fParticleGun->SetParticleDefinition(particle);
	    
	    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
	    
	    //momentum = 1.*MeV;
	    //sigmaMomentum = 100.*MeV;
	    
	    mass = particle->GetPDGMass();
	    //Ekin = std::sqrt(pp*pp+mass*mass) - mass;
	    Ekin = fParticleGun->GetParticleEnergy();

	    //sigmaAngle = 140.*deg;
	    yPos = (G4UniformRand()-0.5)*(worldBox->GetYHalfLength());
	    xPos = (G4UniformRand()-0.5)*(worldBox->GetXHalfLength());
	    zPos = worldZHalfLength + 5.0*m;

	    fParticleGun->SetParticlePosition(G4ThreeVector(xPos, yPos, zPos)); 
	    
	    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(-1, 0, 0));
	    
	    fParticleGun->GeneratePrimaryVertex(anEvent);
	} else {
	G4cout << "Dummy! you need to pick a proper event type. Choices are: 0, 1 or 2 for now." << G4endl;
    }

    // Set gun position
    //  fParticleGun
    //    ->SetParticlePosition(G4ThreeVector(0., 0., -worldZHalfLength));
    //  fParticleGun
    //    ->SetParticlePosition(G4ThreeVector(0., 0., -worldZHalfLength));
    
    //  fParticleGun->GeneratePrimaryVertex(anEvent);
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

