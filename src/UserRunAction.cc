#include "UserRunAction.hh"
#include "G4Run.hh"
#include "UserRun.hh"
#include "G4Timer.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "ConfigFile.hh"
// Change this header if you want different AnalysisManager output (e.g. XML)
//#include "g4root.hh"
#include "G4AnalysisManager.hh"

#include <fstream>
#include <vector>
#include <string>


//UserRunAction::~UserRunAction()
//{   delete fTimer;
//    delete G4AnalysisManager::Instance();
//}

G4Run* UserRunAction::GenerateRun()
{
    return new UserRun;
}


// Defining actions performed at the beginning and/or the end of each run

void UserRunAction::BeginOfRunAction(const G4Run* run)
{
    // Create analysis manager
    // Notice: it must be done the same way in master and workers
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetFirstNtupleId(1);
    // Ntuple merging (only for Geant v. 4.10.03 or higher)
    analysisManager->SetNtupleMerging(true);
    
//    if (IsMaster()){
//        analysisManager->CreateNtuple("Keyword", "Keyword");
//        analysisManager->CreateNtupleDColumn("RunID");
//        analysisManager->CreateNtupleDColumn("NumberOfEvents");
//        analysisManager->FinishNtuple();
//    }
//    else
    {
/*        analysisManager->CreateNtuple("Events", "Events");
        analysisManager->CreateNtupleDColumn("EventID");
        analysisManager->CreateNtupleDColumn("En_dep");
        analysisManager->CreateNtupleDColumn("Scint_ID");
        analysisManager->CreateNtupleDColumn("SDD_ID");
        analysisManager->CreateNtupleDColumn("SDD_Cell");
        analysisManager->CreateNtupleDColumn("X_Primary");
        analysisManager->CreateNtupleDColumn("Y_Primary");
        analysisManager->CreateNtupleDColumn("Z_Primary");
        analysisManager->CreateNtupleDColumn("Theta_Primary");
        analysisManager->CreateNtupleDColumn("Phi_Primary");
        analysisManager->CreateNtupleDColumn("En_Primary");
        analysisManager->CreateNtupleDColumn("Event_time");
        analysisManager->FinishNtuple();*/

        analysisManager->CreateNtuple("Events", "Events");
        analysisManager->CreateNtupleDColumn("EventID");
        analysisManager->CreateNtupleDColumn("En_dep");
        analysisManager->CreateNtupleIColumn("Scint_ID");
        analysisManager->CreateNtupleDColumn("X_Primary");
        analysisManager->CreateNtupleDColumn("Y_Primary");
        analysisManager->CreateNtupleDColumn("Z_Primary");
        analysisManager->CreateNtupleDColumn("Theta_Primary");
        analysisManager->CreateNtupleDColumn("Phi_Primary");
        analysisManager->CreateNtupleDColumn("En_Primary");
        analysisManager->CreateNtupleDColumn("Event_time");
        analysisManager->CreateNtupleDColumn("X_Detected");
        analysisManager->CreateNtupleDColumn("Y_Detected");
        analysisManager->CreateNtupleDColumn("Z_Detected");
        analysisManager->FinishNtuple();

    }
    analysisManager->OpenFile("scorefile.root");
    
    
    if(IsMaster())
    {
        // The run ID is printed at the beginning of each master run
        G4cout << "INFORMATION: Run No. " << run -> GetRunID() << " start." << G4endl;
        fTimer->Start();
//        analysisManager->FillNtupleDColumn(0, run->GetRunID());
//        analysisManager->FillNtupleDColumn(1, run->GetNumberOfEventToBeProcessed());
//        analysisManager->AddNtupleRow();
    }
}


void UserRunAction::EndOfRunAction(const G4Run* run)
{
    // Close and write root file (one for each thread)
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
    
    if (IsMaster()){
        fTimer->Stop();
        // The run ID and the number of processed events are printed at the end of each run
        G4int NbOfEvents = run -> GetNumberOfEvent();
        if (NbOfEvents == 0) return;
        G4cout << "INFORMATION: Run No " << run -> GetRunID() << " end." << G4endl;
        G4cout << "INFORMATION: Number of events = " << NbOfEvents << G4endl;
        G4cout << "INFORMATION: Elapsed time = " << *fTimer << G4endl;
    }
}
