#include "PrimaryGeneratorAction.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"

#include "WriteToFile.hh"

//using namespace CLHEP;

PrimaryGeneratorAction::PrimaryGeneratorAction() {
    particleGun = new G4ParticleGun(1); // 1 particle per event

    // Get particle definition
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle("gamma");

    // Set particle properties
    particleGun->SetParticleDefinition(particle);
    particleGun->SetParticleEnergy(50. * keV); // Set the energy
        // Output for debug
    std::ofstream outputFile("output.txt");
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete particleGun;
}

void PrimaryGeneratorAction::GenerateVertex(G4ThreeVector& vertex) {
    vertex.setX(0.*cm);
    vertex.setY(10.*cm);
    vertex.setZ(0.*cm);
}

void PrimaryGeneratorAction::GenerateDirection(G4ThreeVector& direction) {
    direction.setX(0.);
    direction.setY(-1.);
    direction.setZ(0.);
}

void PrimaryGeneratorAction::GenerateSquare(G4double number) {
    G4cout << "Hello " << number * number << G4endl;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    G4ThreeVector vertex;
    GenerateVertex(vertex);
    particleGun->SetParticlePosition(vertex);

    G4ThreeVector direction;
    GenerateDirection(direction);
    particleGun->SetParticleMomentumDirection(direction);
    
    // Output for debug
    /*
    std::ostringstream strValue;
    strValue << vertex.getX()/cm << " " << vertex.getY()/cm << " " << vertex.getZ()/cm;
    testOutput.print(strValue.str());
    */

    // Generate the particle
    particleGun->GeneratePrimaryVertex(anEvent);
}
