#ifndef SDDDetectorHit_HH
#define SDDDetectorHit_HH

#include <vector>
#include <utility>
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "tls.hh"

// General purpose:
//  Represents a hit and contains relevant information about the hit.
//  (The user implementation should provide functionalities which allow to 
//  assign and retrieve hit information).
//  Hit objects can be created in the sensitive detector, and may be used
//  at the end of events to accumulate hit information.
//
// Purpose of the class in this implementation:
//  Represents a hit in the detector, where the relevant hit information is 
//  the energy deposited by the particle and its location


// G4VHit is the abstract base class for creating hit objects.

class SDDDetectorHit : public G4VHit {
	public:
		SDDDetectorHit();							// Default constructor
		SDDDetectorHit(G4int xp);                   // Alternate constructor
		virtual ~SDDDetectorHit();					// Destructor

		
		// Assignment and comparison operators:  
		const SDDDetectorHit& operator= (const SDDDetectorHit& right);
		int operator==(const SDDDetectorHit& right) const;

		
		// The hit class has user-defined new and delete operators to speed up
		// the generation and deletion of hits objects:
		inline void* operator new(size_t);
		inline void  operator delete(void* hit);

		
		// The G4VHit provides two methods, which can be overloaded by the user to
		// visualize hits or to print information about hits. 
		// Here, these methods are not used (dummy implementation):
		virtual void Draw() { }
		virtual void Print() { }

		
		// The current hit object can be used to keep track of:
		// a. The  energy deposit
		// b. The ID of the sector of deposit
        // c. The coordinates of the voxel in which the energy deposit happened
		
		// The following methods allow to set, accumulate and get the energy deposit:
		inline void SetEnergyDeposit(G4double energy) {energyDeposit = energy;}
		inline void AddEnergyDeposit(G4double energy) {energyDeposit += energy;}
		inline G4double GetEnergyDeposit() 			  {return energyDeposit;}
		
		// The following methods allow to set and get the ID of the readout sector:
		inline void SetSDDID(G4int id) 			  {sddID = id;}
		inline G4int GetSDDID() 				  {return sddID;}
        inline void SetSDDCell(G4int id)          {sddCell = id;}
        inline G4int GetSDDCell()                 {return sddCell;}

	
		// The following methods allow to set and get the time:
		inline void SetTime(G4double t) 		      {globaltime = t;}
		inline G4int GetTime() 					      {return globaltime;}

	private:
		G4double energyDeposit;
		G4int sddID;
        G4double sddCell;
        G4double locpixelY;

		G4double globaltime;
	
};


typedef G4THitsCollection<SDDDetectorHit> SDDDetectorHitsCollection;
extern G4ThreadLocal G4Allocator<SDDDetectorHit>* SDDDetectorHitAllocator;


inline void* SDDDetectorHit::operator new(size_t) {
    // Implementation of the new operator
    if(!SDDDetectorHitAllocator) SDDDetectorHitAllocator = new G4Allocator<SDDDetectorHit>;
    return (void*) SDDDetectorHitAllocator->MallocSingle();
}


inline void SDDDetectorHit::operator delete(void* hit) {
    // Implementation of the delete operator
    SDDDetectorHitAllocator->FreeSingle((SDDDetectorHit*) hit);
}

#endif // SDDDetectorHit_HH
