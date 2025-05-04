#include <iostream>
#include <iterator>
#include <string>

#include "club_info_analyzer.hpp"

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

    ss >> res.ntables;
    if (!ss || !IsEmpty(ss) || res.ntables <= 0) {
        throw InputDataFormatError(line_number, std::move(line_content));
    }

    // line 2: time_open, time_close
    if (!ReadNextLine()) {
        throw InputDataFormatError(line_number);
    }

    ss >> res.time_open >> res.time_close;
    if (!ss || !IsEmpty(ss)) {
        throw InputDataFormatError(line_number, std::move(line_content));
    }

    // check proper timing of range of work
    if (res.time_open >= res.time_close) {
        throw InputDataFormatError(line_number, std::move(line_content));
    }

    // line 3: cost_per_hour
    if (!ReadNextLine()) {
        throw InputDataFormatError(line_number);
    }

    ss >> res.cost_per_hour;
    if (!ss || !IsEmpty(ss) || res.cost_per_hour <= 0) {
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
            auto table_number = event.GetBody<body::ClientTable>().table_number;
            if (table_number < 1 || table_number > res.ntables)  {
                throw InputDataFormatError(line_number, std::move(line_content));
            }
        }

        res.events.push_back(std::move(event));
    }

    return res;
};

#if 0

OutputData ProcessInputData(const InputData &data) {
    OutputData res;

    res.time_start = data.time_open;
    res.time_end   = data.time_close;

    std::unordered_set<std::string> clients_in_club;

    for (auto &&event : data.events) {
        // res.events.push_back(event.Clone());

        switch (event.GetId()) {
            using enum EventId;

        case IN_CLIENT_CAME: {
            const auto &name = event.GetBody<BodyTypeForId<IN_CLIENT_CAME>>().name;
            if (clients_in_club.contains(name)) {

            }
        }
        case IN_CLIENT_START:
        case IN_CLIENT_WAIT:
        case IN_CLIENT_GONE:

        case OUT_CLIENT_GONE:
        case OUT_CLIENT_START:
        case OUT_ERROR:
        case UNKNOWN:
            assert(false && "ProcessInputData: output or unknown event in a list of input events");
        }
    }

    return res;
}

#endif
