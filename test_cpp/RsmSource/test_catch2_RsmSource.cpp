#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "RsmSource.hh"
#include <fstream>

// Helper function to create a sample CSV file for testing
void CreateSampleCSV(const std::string& filename) {
    std::ofstream file(filename);
    file << "event_id,time,angle,pol_x,pol_y,pol_z,energy\n";
    file << "0,0.001,0.1,1.0,0.0,0.0,50.0\n";
    file << "1,0.002,0.2,0.0,1.0,0.0,50.0\n";
    file << "2,0.003,0.3,0.0,0.0,1.0,50.0\n";
    file.close();
}

TEST_CASE("RsmSource ReadCSV", "[RsmSource]") {
    CreateSampleCSV("test_events.csv");

    RsmSource rsmSource;
    rsmSource.ReadCSV("test_events.csv");

    const std::vector<EventData>& events = rsmSource.GetEvents();
    REQUIRE(events.size() == 3);

    REQUIRE(events[0].event_id == 0);
    REQUIRE(events[0].time == Approx(0.001));
    REQUIRE(events[0].angle == Approx(0.1));
    REQUIRE(events[0].pol_x == Approx(1.0));
    REQUIRE(events[0].pol_y == Approx(0.0));
    REQUIRE(events[0].pol_z == Approx(0.0));
    REQUIRE(events[0].energy == Approx(50.0));

    REQUIRE(events[1].event_id == 1);
    REQUIRE(events[1].time == Approx(0.002));
    REQUIRE(events[1].angle == Approx(0.2));
    REQUIRE(events[1].pol_x == Approx(0.0));
    REQUIRE(events[1].pol_y == Approx(1.0));
    REQUIRE(events[1].pol_z == Approx(0.0));
    REQUIRE(events[1].energy == Approx(50.0));

    REQUIRE(events[2].event_id == 2);
    REQUIRE(events[2].time == Approx(0.003));
    REQUIRE(events[2].angle == Approx(0.3));
    REQUIRE(events[2].pol_x == Approx(0.0));
    REQUIRE(events[2].pol_y == Approx(0.0));
    REQUIRE(events[2].pol_z == Approx(1.0));
    REQUIRE(events[2].energy == Approx(50.0));

    std::remove("test_events.csv");
}

TEST_CASE("RsmSource GenerateEvent", "[RsmSource]") {
    RsmSource rsmSource;

    EventData event1 = rsmSource.GenerateEvent();
    REQUIRE(event1.event_id == 0);
    REQUIRE(event1.time == Approx(0.0));
    REQUIRE(event1.angle == Approx(0.0));
    REQUIRE(event1.pol_x == Approx(0.0));
    REQUIRE(event1.pol_y == Approx(0.0));
    REQUIRE(event1.pol_z == Approx(0.0));
    REQUIRE(event1.energy == Approx(0.0));

    EventData event2 = rsmSource.GenerateEvent();
    REQUIRE(event2.event_id == 1);
    REQUIRE(event2.time == Approx(0.0));
    REQUIRE(event2.angle == Approx(0.0));
    REQUIRE(event2.pol_x == Approx(0.0));
    REQUIRE(event2.pol_y == Approx(0.0));
    REQUIRE(event2.pol_z == Approx(0.0));
    REQUIRE(event2.energy == Approx(0.0));
}