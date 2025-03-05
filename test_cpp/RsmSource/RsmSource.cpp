#include "RsmSource.hh"
#include <fstream>
#include <sstream>
#include <iostream>

void RsmSource::ReadCSV(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;

    // Clear any existing data
    events.clear();

    // Skip the header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::istringstream ss(line);
        EventData event;
        std::string value;

        std::getline(ss, value, ',');
        event.event_id = std::stoi(value);
        std::getline(ss, value, ',');
        event.time = std::stod(value);
        std::getline(ss, value, ',');
        event.angle = std::stod(value);
        std::getline(ss, value, ',');
        event.pol_x = std::stod(value);
        std::getline(ss, value, ',');
        event.pol_y = std::stod(value);
        std::getline(ss, value, ',');
        event.pol_z = std::stod(value);
        std::getline(ss, value, ',');
        event.energy = std::stod(value);

        events.push_back(event);
    }

    if (file.is_open()) {
        file.close();
    }

    std::cout << "Read " << events.size() << " events from " << filename << std::endl;
}

EventData RsmSource::GenerateEvent() { //dummy. To implement this function see the .py example
    EventData event;
    event.event_id = next_event_id++;
    event.time = 0.0;
    event.angle = 0.0;
    event.pol_x = 0.0;
    event.pol_y = 0.0;
    event.pol_z = 0.0;
    event.energy = 0.0;
    return event;
}