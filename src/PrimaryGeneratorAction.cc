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

void PrimaryGeneratorAction::GenerateVertex(G4double source_h, G4double source_r, G4double source_alpha, G4ThreeVector &vertex_center, G4ThreeVector& vertex) {
    G4double y = source_h + 4.*cm ; // TdDO: check the exact position of the detector surface (detector center = 10.5*cm?)
    G4double z = source_r * std::cos(source_alpha); 
    G4double x = source_r * std::sin(source_alpha);
    vertex_center.setX(x);
    vertex_center.setY(y);
    vertex_center.setZ(z);

    G4double beam_size1{10.*cm}, beam_size2{10.*cm};
    G4double x_move = -beam_size1/2. + beam_size1*G4UniformRand();
    G4double y_move = 0;
    G4double z_move = -beam_size2/2. + beam_size2*G4UniformRand();
    G4ThreeVector translation(x_move, y_move, z_move);

    vertex = vertex_center + translation;
}

void PrimaryGeneratorAction::GenerateDirection(G4double detector_h, G4ThreeVector vertex, G4ThreeVector& direction) {
    // Define the center of the detector surface
    G4ThreeVector detector_center;
    detector_center.setX(0.);
    detector_center.setY(detector_h);
    detector_center.setZ(0.);

    direction = detector_center - vertex;

    // Normalize the direction vector
    direction = direction.unit();
}

void PrimaryGeneratorAction::GenerateSquare(G4double number) {
    G4cout << "Hello " << number * number << G4endl;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
    G4ThreeVector vertex_center, vertex;
    // Retrieve the event ID
    G4int eventID = anEvent->GetEventID();
    // Map the event ID to an angle in degrees
    G4int angleDegrees = (eventID*10) % 360; // Wrap around after 360
    // Convert the angle to radians
    G4double angleRadians = angleDegrees * (M_PI / 180.0);

    // DEBUG
    angleRadians = M_PI/2.;

    //GenerateVertex(6.*cm, 2.*cm, CLHEP::pi/2., vertex);
    GenerateVertex(6.*cm, 3.*cm, angleRadians, vertex_center, vertex);
    
    particleGun->SetParticlePosition(vertex);

    G4ThreeVector direction;
    GenerateDirection(4.*cm, vertex_center, direction);
    particleGun->SetParticleMomentumDirection(direction);

    // Arbitrary polarization of the particle. It will be correctly defined in the 
    // input csv file.

    // Step 1: choose an arbitrary vector that is not parallel to the direction vector
    G4ThreeVector arbitraryVector(1.0, 0.0, 0.0);
    if (std::abs(direction.dot(arbitraryVector)) > 0.9) {
        arbitraryVector = G4ThreeVector(0.0, 1.0, 0.0);
    }
    // Step 2: use the cross product to find a perpendicular vector
    G4ThreeVector polarization = direction.cross(arbitraryVector);    
    // Step3 : normalize the polarization vector to ensure it has unit length
    polarization = polarization.unit();
    // Define the polarization vector
    particleGun->SetParticlePolarization(polarization);
    
    #ifdef DEBUG
    std::ostringstream strValue;
    strValue << vertex.getX()/cm << " " << vertex.getY()/cm << " " << vertex.getZ()/cm;
    testOutput.print(strValue.str());
    #endif

    // Generate the particle
    particleGun->GeneratePrimaryVertex(anEvent);
}
