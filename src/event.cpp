#include <cassert>

#include "event.hpp"

#include "utils/string.hpp"
#include "utils/stream_operators.hpp"

bool IsInputEventId(int val) {
    using enum EventId;
    return static_cast<int>(IN_CLIENT_CAME) <= val && val <= static_cast<int>(IN_CLIENT_GONE);
}

bool IsOutputEventId(int val) {
    using enum EventId;
    return static_cast<int>(OUT_CLIENT_GONE) <= val && val <= static_cast<int>(OUT_ERROR);
}

bool IsEventId(int val) {
    return IsInputEventId(val) || IsOutputEventId(val) || val == static_cast<int>(EventId::UNKNOWN);
}

void Dump(std::ostream &os, const Event &src) {
    Dump(os, src.GetTime());
    os << " " << static_cast<int>(src.GetId()) << " ";
    if (src.GetId() != EventId::UNKNOWN) {
        src.GetBodySerializer().Dump(os);
    }
}

std::istream &Load(std::istream &is, Event &dest) {
    TimeStamp time;
    is >> time;

    if (!is) {
        return is;
    }

    std::string id;
    is >> id;
    if (!IsDigit(id)) {
        is.setstate(std::ios::failbit);
        return is;
    }

    int event_id = std::stoi(id);
    if (!IsInputEventId(event_id)) {
        is.setstate(std::ios::failbit);
        return is;
    }

    Event event;
    switch (static_cast<EventId>(event_id)) {
        using enum EventId;

        case IN_CLIENT_CAME: {
            ClientInfo client_info;
            is >> client_info;
            if (!is) {
                return is;
            }
            event = Event::Create<IN_CLIENT_CAME>(time, client_info);
            break;
        }
        case IN_CLIENT_WAIT: {
            ClientInfo client_info;
            is >> client_info;
            if (!is) {
                return is;
            }
            event = Event::Create<IN_CLIENT_WAIT>(time, client_info);
            break;
        }
        case IN_CLIENT_START: {
            ClientTable client_table;
            is >> client_table;
            if (!is) {
                return is;
            }
            event = Event::Create<IN_CLIENT_START>(time, client_table);
            break;
        }
        case IN_CLIENT_GONE: {
            ClientInfo client_info;
            is >> client_info;
            if (!is) {
                return is;
            }
            event = Event::Create<IN_CLIENT_GONE>(time, client_info);
            break;
        }
        case UNKNOWN:
        case OUT_CLIENT_GONE:
        case OUT_CLIENT_START:
        case OUT_ERROR:
            assert(false && "event_id is not input event");
    }

    dest = std::move(event);
    return is;
}