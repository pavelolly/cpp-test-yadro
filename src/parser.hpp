#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <concepts>

#include "event.hpp"

struct ComputerClubInfo {
    int ntables;
    Event::TimeStamp time_open;
    Event::TimeStamp time_close;
    int cost_per_hour;
};

struct DailyEvents {
    ComputerClubInfo club_info;
    std::vector<Event> events;

    static DailyEvents Load(std::istream &is) {
        int current_line_number = 1;
        std::string current_line {};

        DailyEvents daily_events;

        
    }
};