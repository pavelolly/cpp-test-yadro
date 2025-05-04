#include <iostream>
#include <iterator>
#include <string>
#include <algorithm>
#include <queue>
#include <cassert>

#include "club_info_analyzer.hpp"
#include "utils/stream_operators.hpp"
#include "errors.hpp"

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
        ss = std::istringstream { line_content };

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
        
        // check input event
        if (!IsInputEventId(static_cast<int>(event.GetId()))) {
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


OutputData ProcessInputData(const InputData &data) {
    OutputData res;

    res.time_start = data.time_open;
    res.time_end   = data.time_close;

    /* these store "references" to somewehre in data.events */

    // if clients_in_club[name] == 0 then client is in the club but not using any table
    // names are required in alphabetical order
    std::map<std::string_view, int> clients_in_club;

    // busy_tables[0] is dummy for convinience (table_numbers in events are in range 1..ntables)
    std::vector<std::string_view> busy_tables(data.ntables + 1);
    std::queue<std::string_view> clients_queue;

    auto ClientEnters = [&](TimeStamp time, std::string_view name) {
        if (clients_in_club.contains(name)) {
            res.AddEvent<EventId::OUT_ERROR>(time, { error::YOU_SHALL_NOT_PASS });
            return;
        } 
        if (time < data.time_open || time > data.time_close) {
            res.AddEvent<EventId::OUT_ERROR>(time, { error::NOT_OPEN_YET });
            return;
        }

        clients_in_club.insert({name, 0});
    };

    auto ClientStarts = [&](TimeStamp time, std::string_view name, int table) {
        if (!clients_in_club.contains(name)) {
            res.AddEvent<EventId::OUT_ERROR>(time, { error::CLIENT_UNKNOWN });
            return;
        }

        if (!busy_tables[table].empty() && busy_tables[table] != name) {
            res.AddEvent<EventId::OUT_ERROR>(time, { error::PLACE_IS_BUSY });
            return;
        }

        clients_in_club[name] = table;
        busy_tables[table] = name;
        return;
    };

    auto ClientLeaves = [&](TimeStamp time, std::string_view name) {
        auto it = clients_in_club.find(name);
        if (it == clients_in_club.end()) {
            res.AddEvent<EventId::OUT_ERROR>(time, { error::CLIENT_UNKNOWN });
            return;
        }

        if (int table = it->second; table != 0) {
            busy_tables[it->second] = std::string_view{};
            
            if (!clients_queue.empty()) {
                auto queued = clients_queue.back();
                clients_queue.pop();

                res.AddEvent<EventId::OUT_CLIENT_START>(time, { std::string(queued), table });
                ClientStarts(time, queued, table);
            }
        }
        clients_in_club.erase(it);
    };

    auto ClientWaits = [&](TimeStamp time, std::string_view name) {
        if (std::any_of(busy_tables.begin() + 1, busy_tables.end(), [](std::string_view name) { return name.empty(); })) {
            res.AddEvent<EventId::OUT_ERROR>(time, { error::CAN_WAIT_NO_LONGER });
            return;
        }

        if (clients_queue.size() > (std::size_t)data.ntables) {
            res.AddEvent<EventId::OUT_CLIENT_GONE>(time, { std::string(name) });
            ClientLeaves(time, name);
            return;
        }

        clients_queue.push(name);
    };
    
    std::for_each(data.events.begin(), data.events.end(), [&](const Event &event) {
        res.events.push_back(event);

        auto id   = event.GetId();
        auto time = event.GetTime();

        switch (id) {
            using enum EventId;

        case IN_CLIENT_CAME: {
            const auto &[name] = event.GetBody<BodyTypeForId<IN_CLIENT_CAME>>();

            ClientEnters(time, name);
            return;
        }
        case IN_CLIENT_START: {
            const auto &[name, table] = event.GetBody<BodyTypeForId<IN_CLIENT_START>>();

            ClientStarts(time, name, table);
            return;
        }        
        case IN_CLIENT_WAIT: {
            const auto &[name] = event.GetBody<BodyTypeForId<IN_CLIENT_WAIT>>();

            ClientWaits(time, name);
            return;
        }
        case IN_CLIENT_GONE: {
            const auto &[name] = event.GetBody<BodyTypeForId<IN_CLIENT_WAIT>>();

            ClientLeaves(time, name);
            return;
        }
        case OUT_CLIENT_GONE:
        case OUT_CLIENT_START:
        case OUT_ERROR:
        case UNKNOWN:
            assert(false && "ProcessInputData: output or unknown event in a list of input events");
        }
    });

    for (const auto& [name, _] : clients_in_club) {
        res.AddEvent<EventId::OUT_CLIENT_GONE>(data.time_close, { std::string(name) });
    }

    return res;
}

