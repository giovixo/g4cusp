#include "SDDSensitiveDetector.hh"
#include "SDDDetectorHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4VProcess.hh"
#include "G4StepPoint.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4VSolid.hh"
#include "G4VisExtent.hh"

#include <iostream>

// Constructor.
SDDSensitiveDetector::SDDSensitiveDetector(G4String SDuniqueName) :
G4VSensitiveDetector(SDuniqueName),
numberOfCellsX(12),
numberOfCellsY(10)
{
    collectionName.insert(SDuniqueName);
    initArray();
}



// Method for the array allocation
void SDDSensitiveDetector::initArray()
{
    // Dynamic allocation of 2-D arrays
    // We use the sizeof() a pointer to ensure 32 and 64-bit portability
    CellID = (int **) std::calloc(numberOfCellsX, sizeof(*CellID));
    for(int iRow=0; iRow < numberOfCellsX; iRow++)
    {
        CellID[iRow] = (int *) std::calloc(numberOfCellsY, sizeof(*CellID));
    }
}



// Destructor
SDDSensitiveDetector::~SDDSensitiveDetector()
{
    std::free(CellID);
}



void SDDSensitiveDetector::Initialize(G4HCofThisEvent* hitsCollEvent)
{
    // This method is called at each new event, and it gets the HCofThisEvents
    // for the event.
    hitsCollection = new SDDDetectorHitsCollection(SensitiveDetectorName, collectionName[0]);
    
    hitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hitsCollEvent->AddHitsCollection(hitsCollectionID, hitsCollection);
    
    // Initialize vector of cell hits
    for(int iRow=0; iRow < numberOfCellsX; iRow++)
    {
        for(G4int iCol=0; iCol < numberOfCellsY; iCol++)
        {
            CellID[iRow][iCol] = -1;
        }
    }
}


G4bool SDDSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* ROhist)
{
    // This method is called when a particle goes through a sensitive
    // detector. It possibly creates a new hit, and add its to the collection.
    // It eventually returns a boolean value.
    // The arguments it gets are the G4Step and the G4TouchableHistory.
    //
    // The touchable history that we get in this method contains the full information about the
    // genealogy of the current volume in the read-out geometry. One can use the touchable to
    // access the information on the position/rotation of a given volume.
    
    //    if(!ROhist) return false;
    
    // Retrieve the energy deposited from the step
    G4double energyDeposit = step -> GetTotalEnergyDeposit();
    G4double energy_threshold = 0.01 * keV;
    if (energyDeposit < energy_threshold) return false;
    
    G4Track* track = step -> GetTrack();
    G4double globaltime = track -> GetGlobalTime();

    G4double x_center = 0 * mm;
    G4double y_center = 0 * mm;
    G4double z_center = 0 * mm;
    G4VSolid *targetSolid = 0;
    G4VisExtent targetVisExtent;
    
    
    // debug code
//    G4StepPoint* point1 = step->GetPreStepPoint();
//    const G4VProcess* aProcess1 = point1->GetProcessDefinedStep();
//    G4cout << aProcess1->GetProcessName() << G4endl;
//    G4StepPoint* point2 = step->GetPostStepPoint();
//    const G4VProcess* aProcess2 = point2->GetProcessDefinedStep();
//    G4cout << aProcess2->GetProcessName() << G4endl;
    
    // Get the interaction volume
    G4String volumeName = track -> GetVolume() -> GetName();

    targetSolid = track -> GetVolume() -> GetLogicalVolume() -> GetSolid();
    targetVisExtent = targetSolid->GetExtent();

    x_center = targetVisExtent.GetExtentCenter()[0];
    y_center = targetVisExtent.GetExtentCenter()[1];
    z_center = targetVisExtent.GetExtentCenter()[2];

    //-> G4int volumeNumber = std::stoi( volumeName.substr(volumeName.size()-2, volumeName.size()-1));
    // Placeholder. Fix the following line
    G4int volumeNumber = 1;
    // G4int volumeNumber = std::stoi( volumeName.substr(volumeName.size()-11, 2) );
    G4String volumeNum = volumeName.substr(volumeName.size()-11, 2);
    G4cout << "*** DEBUG *** volumeName: " << volumeName << " VolumeNum: " << volumeNum << G4endl;
    G4cout << "*** DEBUG *** volume position (x, y, z of the center): " << x_center << " " << y_center << " " << z_center << G4endl;

    // Get position
    G4ThreeVector position = track -> GetPosition();
//    G4double position_x = position.x();
//    G4double position_y = position.y();
//    G4double position_z = position.z();
//    G4cout << "*** DEBUG *** volumeName " << volumeName << " volumeNumber " << volumeNumber << " energyDeposit " << energyDeposit/keV <<  G4endl;
//    G4cout << "*** DEBUG *** position " << position_x/mm << " " << position_y/mm << " " << position_z/mm << G4endl;
    
    G4double x_c = 0 * mm;
    G4double y_c = 0 * mm;

    if (volumeNumber == 0)
    {
        x_c = 11.25 * mm;
        y_c = 29.70 * mm;
    }
    else if (volumeNumber == 1)
    {
        x_c = 11.25 * mm;
        y_c = 70.30 * mm;
    }
    else if (volumeNumber == 2)
    {
        x_c = 26.75 * mm;
        y_c = 29.70 * mm;
    }
    else if (volumeNumber == 3)
    {
        x_c = 26.75 * mm;
        y_c = 70.30 * mm;
    }
    else if (volumeNumber == 4)
    {
        x_c = 42.25 * mm;
        y_c = 27.65 * mm;
    }
    else if (volumeNumber == 5)
    {
        x_c = 42.25 * mm;
        y_c = 72.35 * mm;
    }
    else if (volumeNumber == 6)
    {
        x_c = 57.75 * mm;
        y_c = 27.65 * mm;
    }
    else if (volumeNumber == 7)
    {
        x_c = 57.75 * mm;
        y_c = 72.35 * mm;
    }
    else if (volumeNumber == 8)
    {
        x_c = 73.25 * mm;
        y_c = 29.70 * mm;
    }
    else if (volumeNumber == 9)
    {
        x_c = 73.25 * mm;
        y_c = 70.30 * mm;
    }
    else if (volumeNumber == 10)
    {
        x_c = 88.75 * mm;
        y_c = 29.70 * mm;
    }
    else if (volumeNumber == 11)
    {
        x_c = 88.75 * mm;
        y_c = 70.30 * mm;
    }

    G4double relative_position_x = x_c - position.x();
    G4double relative_position_y = y_c - position.y();
    
    G4int x_hit_side = 0;
    G4int y_hit_side = 0;

    if (relative_position_x >= 0) {x_hit_side = 1;}
    
    if (relative_position_y < -11.6*mm) {y_hit_side = 4;}
    else if (relative_position_y >= -11.6*mm && relative_position_y < -3.72*mm) {y_hit_side = 3;}
    else if (relative_position_y >= -3.72*mm && relative_position_y < 3.72*mm) {y_hit_side = 2;}
    else if (relative_position_y >= 3.72*mm && relative_position_y < 11.6*mm) {y_hit_side = 1;}
    else if (relative_position_y >= 11.6*mm) {y_hit_side = 0;}

    G4int copyIDinX   =  volumeNumber;
    G4int copyIDinY   =  y_hit_side + 5*x_hit_side;
    G4int icell = 0;

    
    if(CellID[copyIDinX][copyIDinY]==-1) // if no energy depositions before in this cell
    {
        // Now we create a new hit object, corresponding to the cell, and fill it with values to be stored
        SDDDetectorHit* hit = new SDDDetectorHit();
        hit -> SetEnergyDeposit(energyDeposit);
        hit -> SetSDDID(volumeNumber);
        hit -> SetSDDCell(copyIDinY);

        hit -> SetTime(globaltime);
        icell = hitsCollection -> insert(hit);
        CellID[copyIDinX][copyIDinY] = icell - 1;
    }
    else // if energy depositions before in this cell
    {
        (*hitsCollection)[CellID[copyIDinX][copyIDinY]] -> AddEnergyDeposit(energyDeposit);
        (*hitsCollection)[CellID[copyIDinX][copyIDinY]] -> SetTime(globaltime);
    }
    return true;
}


void SDDSensitiveDetector::EndOfEvent(G4HCofThisEvent* hitsCollEvent)
{
    //    // Method called at the end of an event.
    //    if(hitsCollectionID < 0) {
    //        hitsCollectionID =
    //        G4SDManager::GetSDMpointer() -> GetCollectionID(hitsCollection);
    //    }
    //    // Finally, the hits collection just defined (with its ID and its hits)
    //    // is registered to the HCOfThisEvent of the current event.
    //    hitsCollEvent -> AddHitsCollection(hitsCollectionID, hitsCollection);
}
