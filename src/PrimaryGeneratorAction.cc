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

void PrimaryGeneratorAction::GenerateVertex(G4double source_h, G4double source_r, G4double source_alpha, G4ThreeVector& vertex) {
    G4double y = source_h + 4.*cm ; // TdDO: check the exact position of the detector surface (detector center = 10.5*cm?)
    G4double z = source_r * std::cos(source_alpha); 
    G4double x = source_r * std::sin(source_alpha);
    vertex.setX(x);
    vertex.setY(y);
    vertex.setZ(z);
}

void PrimaryGeneratorAction::GenerateDirection(G4double detector_h, G4double beam_size1, G4double beam_size2, G4ThreeVector vertex, G4ThreeVector& direction) {
    // Define the center of the detector surface
    G4ThreeVector detector_center;
    detector_center.setX(0.);
    detector_center.setY(detector_h);
    detector_center.setZ(0.);

    direction = (detector_center - vertex).unit();
}

void PrimaryGeneratorAction::GenerateSquare(G4double number) {
    G4cout << "Hello " << number * number << G4endl;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    G4ThreeVector vertex;
    // Retrieve the event ID
    G4int eventID = anEvent->GetEventID();
    // Map the event ID to an angle in degrees
    G4int angleDegrees = (eventID*10) % 360; // Wrap around after 360
    // Convert the angle to radians
    G4double angleRadians = angleDegrees * (M_PI / 180.0);

    //GenerateVertex(6.*cm, 2.*cm, CLHEP::pi/2., vertex);
    GenerateVertex(6.*cm, 3.*cm, angleRadians, vertex);
    
    particleGun->SetParticlePosition(vertex);

    G4ThreeVector direction;
    GenerateDirection(4.*cm, 0., 0., vertex, direction);
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
