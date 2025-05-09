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
    // TODO: rewrite with std::vector
    std::map<int, TableInfo> table_infos;

    template <EventId Id>
    void AddEvent(TimeStamp time, BodyTypeForId<Id> body) {
        events.push_back(Event::Create<Id>(time, std::move(body)));
    }
};

#if 0

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

#else

class InputDataFormatError : public std::runtime_error {
public:
    InputDataFormatError(int)
        : std::runtime_error("")
    {}
    InputDataFormatError(int, std::string line_content)
        : std::runtime_error(std::move(line_content))
    {}
};

#endif // #if 0

InputData LoadInputData(std::istream &is);
OutputData ProcessInputData(const InputData &data);
void DumpOutputData(std::ostream &os, const OutputData &src);
