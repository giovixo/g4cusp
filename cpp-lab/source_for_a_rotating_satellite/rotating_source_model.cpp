#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <string>
#include <sstream>

struct Event {
    int event_id;
    double time;
    double angle;
    double pol_x;
    double pol_y;
    double pol_z;
    double energy;
};

std::vector<Event> generate_events(double energy_input, double photon_rate, double exposure_time) {
    int N = static_cast<int>(photon_rate * exposure_time);
    std::vector<Event> events(N);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<> d(1.0 / photon_rate);

    double omega = 0.01 * (2.0 * M_PI);
    double cumulative_time = 0.0;

    for (int i = 0; i < N; ++i) {
        double time_interval = d(gen);
        cumulative_time += time_interval;
        double angle = omega * cumulative_time;

        events[i] = {
            i,
            cumulative_time,
            angle,
            std::cos(angle),
            0.0,
            std::sin(angle),
            energy_input
        };
    }

    std::cout << N << " photons with energy = " << energy_input << " keV each have been generated." << std::endl;
    return events;
}

void save_to_csv(const std::vector<Event>& events, const std::string& filename) {
    std::ofstream file(filename);
    file << "event_id,time,angle,pol_x,pol_y,pol_z,energy\n";
    for (const auto& event : events) {
        file << event.event_id << ","
             << event.time << ","
             << event.angle << ","
             << event.pol_x << ","
             << event.pol_y << ","
             << event.pol_z << ","
             << event.energy << "\n";
    }
    std::cout << "Events saved to " << filename << std::endl;
}

int main(int argc, char* argv[]) {
    double energy_input = 100.0;
    double photon_rate = 10.0;
    double exposure_time = 10.0;

    if (argc == 1) {
        // Use default values
    } else if (argc == 2 && std::string(argv[1]) == "help") {
        std::cout << "USAGE\n";
        std::cout << "rotating_source_model <energy_input> <photon_rate> <exposure_time>\n";
        return 0;
    } else if (argc == 4) {
        energy_input = std::stod(argv[1]);
        photon_rate = std::stod(argv[2]);
        exposure_time = std::stod(argv[3]);
    } else {
        std::cerr << "Invalid number of arguments. Please run 'rotating_source_model help' for usage." << std::endl;
        return 1;
    }

    std::cout << "Running main function..." << std::endl;
    auto events = generate_events(energy_input, photon_rate, exposure_time);
    save_to_csv(events, "rsm-events.csv");

    return 0;
}