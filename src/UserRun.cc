#include "UserRun.hh"

#include "G4SDManager.hh"
#include "SDDDetectorHit.hh"
#include "DetectorHit.hh"
#include "G4Event.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4UIcommand.hh"
#include "tls.hh"

#include "ConfigFile.hh"

//#include "g4root.hh"
#include "G4AnalysisManager.hh"

UserRun::UserRun()
{
    SCI_hitsCollectionIndex = G4SDManager::GetSDMpointer()->GetCollectionID("SCI");
    SDD_hitsCollectionIndex = G4SDManager::GetSDMpointer()->GetCollectionID("SDD");
}

UserRun::~UserRun() {;}


void UserRun::RecordEvent(const G4Event* event)
{
	// This method specifies the actions that must be performed at the 
	// end of each event (e.g. retrieve information, score, clean up 
	// things, etc.)
	
	// The following line verifies that a valid hits collection has been 
	// found. If not, nothing happens.
	if(SCI_hitsCollectionIndex < 0 && SDD_hitsCollectionIndex < 0) return;

    G4double eventID = event -> GetEventID();
    
    // Now, get the HCofThisEvent: it contains all the hits collections
	// that have been defined.
	G4HCofThisEvent* HCE = event -> GetHCofThisEvent();
    
	// Retrieve the hit collections.
    DetectorHitsCollection* SCI_hitsCollection = 0;
    SDDDetectorHitsCollection* SDD_hitsCollection = 0;

    
	if(HCE) {
        SCI_hitsCollection = (DetectorHitsCollection*)(HCE -> GetHC(SCI_hitsCollectionIndex));
        SDD_hitsCollection = (SDDDetectorHitsCollection*)(HCE -> GetHC(SDD_hitsCollectionIndex));
	}
    
    // If both hit collections are empty, nothing happens.
    int SDD_numberHits = 0;
    int SCI_numberHits = 0;
    if(SCI_hitsCollection) {SCI_numberHits = SCI_hitsCollection -> entries();}
    if(SDD_hitsCollection) {SDD_numberHits = SDD_hitsCollection -> entries();}
    if (SCI_numberHits == 0 && SDD_numberHits == 0) return;


//     Get primary event informations
	G4double en_primary    = 0.*MeV;
	G4double x_primary     = 0.*cm;
	G4double y_primary     = 0.*cm;
	G4double z_primary     = 0.*cm;
	G4double theta_primary = 0.*deg;
	G4double phi_primary   = 0.*deg;
	G4int nVertex = event -> GetNumberOfPrimaryVertex();
//	 G4cout << " *** DEBUG " << " nVertex " << nVertex << G4endl;
	for ( G4int j = 0; j < nVertex; j++) {
		G4int nPart =  event -> GetPrimaryVertex(j)->GetNumberOfParticle();
//		G4cout << " *** DEBUG " << " nPart " << nVertex << G4endl;
		for ( G4int i = 0; i < nPart; i++) {
			G4ThreeVector position = event -> GetPrimaryVertex(j) -> GetPosition();
			G4ThreeVector direction = event -> GetPrimaryVertex(j) -> GetPrimary(i) -> GetMomentum();
			G4double P = direction.mag();
			en_primary = event->GetPrimaryVertex(j)->GetPrimary(i)->GetMass();
			G4double E0 = event->GetPrimaryVertex(j)->GetPrimary(i)->GetG4code()->GetPDGMass();
			en_primary = std::sqrt(P*P+E0*E0);
			en_primary -= E0;
			direction = direction*(1./direction.mag());
			direction = -direction;  // reverse the direction
			phi_primary = direction.phi();
			if (phi_primary <0) phi_primary = phi_primary + twopi;
			theta_primary = direction.theta();
			x_primary=position.x();
			y_primary=position.y();
			z_primary=position.z();
//			G4cout << " *** DEBUG " << " position " << position << G4endl;
//			G4cout << " *** DEBUG " << " direction " << direction << G4endl;
//			G4cout << " *** DEBUG " << " P " << P << G4endl;
//			G4cout << " *** DEBUG " << " en_primary " << en_primary << G4endl;
//			G4cout << " *** DEBUG " << " x " << x_primary << G4endl;
//			G4cout << " *** DEBUG " << " y " << y_primary << G4endl;
//			G4cout << " *** DEBUG " << " z " << z_primary << G4endl;
//			G4cout << " *** DEBUG " << " Theta " << theta_primary/deg << G4endl;
//			G4cout << " *** DEBUG " << " Phi " << phi_primary/deg << G4endl;
		}
	}
    

    // Get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    // Process scintillator hits
    if(SCI_hitsCollection) {

		for(int i = 0; i < SCI_numberHits ; i++) {
			DetectorHit* ahit = (*SCI_hitsCollection)[i];
			G4double energyDeposit = ahit -> GetEnergyDeposit();
            G4int    xpixel    = ahit -> GetScintID();
			G4double lastStepGlobalTime = ahit -> GetTime();
            
            // Write energy deposit and pixel in the ROOT file
/*            analysisManager->FillNtupleDColumn(0, eventID);
            analysisManager->FillNtupleDColumn(1, energyDeposit/keV);
            analysisManager->FillNtupleDColumn(2, xpixel);
            analysisManager->FillNtupleDColumn(3, -1);
            analysisManager->FillNtupleDColumn(4, -1);
            analysisManager->FillNtupleDColumn(5, x_primary/cm);
            analysisManager->FillNtupleDColumn(6, y_primary/cm);
            analysisManager->FillNtupleDColumn(7, z_primary/cm);
            analysisManager->FillNtupleDColumn(8, theta_primary/deg);
            analysisManager->FillNtupleDColumn(9, phi_primary/deg);
            analysisManager->FillNtupleDColumn(10, en_primary/keV);
            analysisManager->FillNtupleDColumn(11, lastStepGlobalTime/ns);
            analysisManager->AddNtupleRow();*/

            analysisManager->FillNtupleDColumn(0, eventID);
            analysisManager->FillNtupleDColumn(1, energyDeposit/keV);
            analysisManager->FillNtupleIColumn(2, xpixel);
            analysisManager->FillNtupleDColumn(3, x_primary/cm);
            analysisManager->FillNtupleDColumn(4, y_primary/cm);
            analysisManager->FillNtupleDColumn(5, z_primary/cm);
            analysisManager->FillNtupleDColumn(6, theta_primary/deg);
            analysisManager->FillNtupleDColumn(7, phi_primary/deg);
            analysisManager->FillNtupleDColumn(8, en_primary/keV);
            analysisManager->FillNtupleDColumn(11, lastStepGlobalTime/ns);
            analysisManager->AddNtupleRow();
            

		}
	}
    
    // Process SDD hits
    if(SDD_hitsCollection) {

        for(int i = 0; i < SDD_numberHits ; i++) {
            SDDDetectorHit* ahit = (*SDD_hitsCollection)[i];
            G4double energyDeposit = ahit -> GetEnergyDeposit();
            G4int    sddID    = ahit -> GetSDDID();
            G4int    sddCell   = ahit -> GetSDDCell();

            G4double lastStepGlobalTime = ahit -> GetTime();

            // Write energy deposit and pixel in the ROOT file
/*            analysisManager->FillNtupleDColumn(0, eventID);
            analysisManager->FillNtupleDColumn(1, energyDeposit/keV);
            analysisManager->FillNtupleDColumn(2, -1);
            analysisManager->FillNtupleDColumn(3, sddID);
            analysisManager->FillNtupleDColumn(4, sddCell);
            analysisManager->FillNtupleDColumn(5, x_primary/cm);
            analysisManager->FillNtupleDColumn(6, y_primary/cm);
            analysisManager->FillNtupleDColumn(7, z_primary/cm);
            analysisManager->FillNtupleDColumn(8, theta_primary/deg);
            analysisManager->FillNtupleDColumn(9, phi_primary/deg);
            analysisManager->FillNtupleDColumn(10, en_primary/keV);
            analysisManager->FillNtupleDColumn(11, lastStepGlobalTime/ns);
            analysisManager->AddNtupleRow();*/

            analysisManager->FillNtupleDColumn(0, eventID);
            analysisManager->FillNtupleDColumn(1, energyDeposit/keV);
            analysisManager->FillNtupleDColumn(2, -1);
            analysisManager->FillNtupleDColumn(3, x_primary/cm);
            analysisManager->FillNtupleDColumn(4, y_primary/cm);
            analysisManager->FillNtupleDColumn(5, z_primary/cm);
            analysisManager->FillNtupleDColumn(6, theta_primary/deg);
            analysisManager->FillNtupleDColumn(7, phi_primary/deg);
            analysisManager->FillNtupleDColumn(8, en_primary/keV);
            analysisManager->FillNtupleDColumn(11, lastStepGlobalTime/ns);
            analysisManager->AddNtupleRow();
        }
    }

    
    
    // Record the event
    G4cout << "---> (Record Event) End of event: " << event -> GetEventID() << G4endl;
    G4Run::RecordEvent(event);
}

void UserRun::Merge(const G4Run* run)
{
    G4cout << "---> Merging files... " << G4endl;
    G4Run::Merge(run);
    G4cout << "---> Merged files. " << G4endl;

}
