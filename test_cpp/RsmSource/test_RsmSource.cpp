#include "RsmSource.hh"
#include <iostream>

int main() {
    // Create an instance of RsmSource
    RsmSource rsmSource;

    // Read events from CSV file
    rsmSource.ReadCSV("events.csv");

    const std::vector<EventData>& events = rsmSource.GetEvents();  // Get the events from the CSV file

    // Print the events to verify the data
    for (const auto& event : events) {
        std::cout << "Event ID: " << event.event_id << ", "
                  << "Time: " << event.time << ", "
                  << "Angle: " << event.angle << ", "
                  << "Pol_x: " << event.pol_x << ", "
                  << "Pol_y: " << event.pol_y << ", "
                  << "Pol_z: " << event.pol_z << ", "
                  << "Energy: " << event.energy << std::endl;
        auto new_event = rsmSource.GenerateEvent(); // generate new events
        std::cout << "Event ID: " << new_event.event_id << ", "
                  << "Time: " << new_event.time << ", "
                  << "Angle: " << new_event.angle << ", "
                  << "Pol_x: " << new_event.pol_x << ", "
                  << "Pol_y: " << new_event.pol_y << ", "
                  << "Pol_z: " << new_event.pol_z << ", "
                  << "Energy: " << new_event.energy << std::endl;
    }

    return 0;
}