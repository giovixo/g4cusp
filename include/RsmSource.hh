#ifndef RSMSOURCE_HH
#define RSMSOURCE_HH

#include <vector>
#include <string>

struct EventData {
    int event_id;
    double time;
    double angle;
    double pol_x;
    double pol_y;
    double pol_z;
    double energy;
};

class RsmSource {
public:
    RsmSource() = default;
    ~RsmSource() = default;

    void ReadCSV(const std::string& filename);
    const std::vector<EventData>& GetEvents() const { return events; }
    EventData GenerateEvent(); // Method to generate an EventData

private:
    std::vector<EventData> events;
    int next_event_id = 0; // Keep track of the next event ID
};

#endif // RSMSOURCE_HH