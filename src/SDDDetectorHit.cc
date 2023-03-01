#include "SDDDetectorHit.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4ThreeVector.hh"

G4ThreadLocal G4Allocator<SDDDetectorHit>* SDDDetectorHitAllocator = 0;

// Default constructor 
SDDDetectorHit::SDDDetectorHit() 
:	energyDeposit(0),
	sddID(0),
    sddCell(0),
	globaltime(0)
{}		



// Alternate constructor
SDDDetectorHit::SDDDetectorHit(G4int xp)
:  	energyDeposit(0),
	sddID(xp),
    sddCell(0),
	globaltime(0)
{;}    



// Destructor
SDDDetectorHit::~SDDDetectorHit() {}		



const SDDDetectorHit& SDDDetectorHit::operator=(const SDDDetectorHit& right) {
	// Assignment operator	
	energyDeposit = right.energyDeposit;
	sddID = right.sddID;
    sddCell = right.sddCell;
	globaltime = right.globaltime; 
	return *this;
}



int SDDDetectorHit::operator==(const SDDDetectorHit& right) const {
	// Comparison operator
	return (this == &right) ? 1 : 0;
}

