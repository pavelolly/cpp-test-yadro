#include "reader.hpp"

#include <iostream>
#include <iterator>
#include <string>

#include "event.hpp"
#include "utils/stream_operators.hpp"

InputData LoadInputData(std::istream &is) {
    int line_number = 0;
    std::string line_content;
    std::istringstream ss;

    InputData res;

    auto ReadNextLine = [&] {
        std::getline(is, line_content);
        if ((is.bad() || is.fail()) && !is.eof()) {
            throw std::runtime_error("LoadInputData: error occured while reading from std::istream");
        }
        ++line_number;
        ss = std::istringstream {line_content};

        return static_cast<bool>(is);
    };

    auto IsEmpty = [](std::istream &is) {
        char dummy;
        is >> dummy;
        return !is;
    };

    // line 1: ntables;
    if (!ReadNextLine()) {
        throw InputDataFormatError(line_number);
    }

    ss >> res.club_info.ntables;
    if (!ss || !IsEmpty(ss)) {
        throw InputDataFormatError(line_number, std::move(line_content));
    }

    // line 2: time_open, time_close
    if (!ReadNextLine()) {
        throw InputDataFormatError(line_number);
    }

    ss >> res.club_info.time_open >> res.club_info.time_close;
    if (!ss || !IsEmpty(ss)) {
        throw InputDataFormatError(line_number, std::move(line_content));
    }

    // check proper timing of range of work
    if (res.club_info.time_open >= res.club_info.time_close) {
        throw InputDataFormatError(line_number, std::move(line_content));
    }

    // line 3: cost_per_hour
    if (!ReadNextLine()) {
        throw InputDataFormatError(line_number);
    }

    ss >> res.club_info.cost_per_hour;
    if (!ss || !IsEmpty(ss)) {
        throw InputDataFormatError(line_number, std::move(line_content));
    }

    // other line: list of events
    while (ReadNextLine()) {
        Event event;
        ss >> event;
        if (!ss || !IsEmpty(ss)) {
            throw InputDataFormatError(line_number, std::move(line_content));
        }

        // check time sequence
        if (!res.events.empty() && res.events.back().GetTime() > event.GetTime()) {
            throw InputDataFormatError(line_number, std::move(line_content));
        }

        // check tables
        if (event.GetId() == EventId::IN_CLIENT_START) {
            auto table_number = event.GetBody<ClientTable>().table_number;
            if (table_number < 1 || table_number > res.club_info.ntables)  {
                throw InputDataFormatError(line_number, std::move(line_content));
            }
        }

        res.events.push_back(std::move(event));
    }

    return res;
};