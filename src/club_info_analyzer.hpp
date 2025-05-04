#pragma once

#include <iostream>
#include <vector>

#include "utils/timestamp.hpp"
#include "event.hpp"

struct ComputerClubInfo {
    int ntables;
    TimeStamp time_open;
    TimeStamp time_close;
    int cost_per_hour;

    bool operator ==(const ComputerClubInfo &) const = default;
};

struct InputData {
    ComputerClubInfo club_info;
    std::vector<Event> events;

    bool operator ==(const InputData &) const = default;
};

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
