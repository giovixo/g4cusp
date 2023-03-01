#include "G4MTRunManager.hh"
#include "G4RunManager.hh"

#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#include "DetectorConstruction.hh"
#include "UserActionInitialization.hh"

#include "PhysicsList.hh"
#include "G4PhysListFactory.hh"
#include "G4VUserPhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "UserRunAction.hh"
#include "UserEventAction.hh"




int main(int argc, char **argv)
{
//    G4Random::setTheEngine(new CLHEP::RanecuEngine);
//    G4Random::setTheSeed(time(0));
    
    // Detect interactive mode (if no arguments) and define UI session
    G4UIExecutive* ui = 0;
    if (argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }

    // Construct the run manager
#ifdef G4MULTITHREADED
    G4MTRunManager * runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(1);
#else
    G4RunManager * runManager = new G4RunManager;
#endif


    // Set mandatory initialization classes
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new PhysicsList());

    // // User action initialization
    runManager->SetUserInitialization(new UserActionInitialization());
    
    // Initialize G4 kernel
    runManager->Initialize();

    
    // Visualization manager construction
    auto visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();
    
    // Get the pointer to the User Interface manager
    auto uiManager = G4UImanager::GetUIpointer();
    
    // Activation of atomic EM de-excitation processes
    uiManager->ApplyCommand("/run/initialize");
//    uiManager->ApplyCommand("/process/em/deexcitation World true true true");
//    uiManager->ApplyCommand("/process/em/fluo true");
//    uiManager->ApplyCommand("/process/em/auger true");
//    uiManager->ApplyCommand("/run/physicsModified");
    
    if (!ui) // Batch mode
    {
        // execute an argument macro file if exist
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        if(argc==3)
        {
            // If there are two batch files (e.g. a General Particle Source macro)
            // load this first
            G4String fileGPS = argv[2];
            uiManager->ApplyCommand(command+fileGPS);
            G4cout << "GPS macro file executed: " << fileGPS << G4endl;
        }
        else
        {
            // Default GPS
            uiManager->ApplyCommand("/gps/particle gamma");
            uiManager->ApplyCommand("/gps/energy 100 keV");
            uiManager->ApplyCommand("/gps/position     0.0  0.0 +50.0 cm");
            uiManager->ApplyCommand("/gps/direction    0.0  0.0 -1.0");
            //uiManager->ApplyCommand("/gps/polarization 1.0  0.0  0.0");
        }
        // Execute batch file
        uiManager->ApplyCommand(command+fileName);
        G4cout << "Batch file executed: " << fileName << G4endl;
    }
    else // GUI or interactive mode
    {
        uiManager->ApplyCommand("/control/execute init_vis.mac");
        
        if (ui->IsGUI()) {
            uiManager->ApplyCommand("/control/execute visGUI.mac");
            uiManager->ApplyCommand("/control/execute visGUI2.mac");
        }
        // start interactive session
        ui->SessionStart();
        delete ui;
    }

    
    // Job termination
    delete visManager;
    delete runManager;
    return 0;
}


