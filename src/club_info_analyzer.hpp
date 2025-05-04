#pragma once

#include <iostream>
#include <vector>
#include <map>

#include "utils/timestamp.hpp"
#include "event.hpp"

struct InputData {
    int ntables;
    TimeStamp time_open;
    TimeStamp time_close;
    int cost_per_hour;
    std::vector<Event> events;

    bool operator ==(const InputData &) const = default;
};

struct TableInfo {
    int earnings;
    TimeStamp time_used;
};

struct OutputData {
    TimeStamp time_start;
    TimeStamp time_end;
    std::vector<Event> events;
    // need table numbers in order
    std::map<int, TableInfo> table_infos;

    template <EventId Id>
    void AddEvent(TimeStamp time, BodyTypeForId<Id> body) {
        events.push_back(Event::Create<Id>(time, std::move(body)));
    }
};

// TODO: better error reports
class InputDataFormatError : public std::runtime_error {
public:
    InputDataFormatError(int line_number)
        : std::runtime_error(std::format("FormatError: Absense of neccessary Line {}", line_number))
    {}
    InputDataFormatError(int line_number, std::string line_content)
        : std::runtime_error(std::format("FormatError: Line {}: '{}'", line_number, std::move(line_content)))
    {}
};

InputData LoadInputData(std::istream &is);
OutputData ProcessInputData(const InputData &data);
