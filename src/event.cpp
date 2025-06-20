#include "event.hpp"

#include <cassert>

#include "utils/string.hpp"
#include "utils/stream_operators.hpp"

namespace internal {

SerializableBody::SerializableBody(const SerializableBody &other)
    : body_(other.body_ ? other.body_->Clone() : nullptr)
{}

SerializableBody &SerializableBody::operator =(const SerializableBody &other) {
    body_ = other.body_ ? other.body_->Clone() : nullptr;
    return *this;
}

bool SerializableBody::Empty() const {
    return !body_;
}

void SerializableBody::Dump(std::ostream &os) const {
    return body_->Dump(os);
}

} // namespace internal

void Dump(std::ostream &os, const Event &src) {
    Dump(os, src.GetTime());
    os << " " << static_cast<int>(src.GetId()) << " ";
    if (src.HasBody()) {
        src.body_.Dump(os);
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
    if (!is || !IsDigit(id)) {
        is.setstate(std::ios::failbit);
        return is;
    }

    int event_id = std::stoi(id);
    if (!IsEventId(event_id)) {
        is.setstate(std::ios::failbit);
        return is;
    }

    switch (static_cast<EventId>(event_id)) {
        using enum EventId;

        // TODO: seems like you could make a macro for each case...
        //       will this worth it though...
        case IN_CLIENT_CAME: {
            BodyTypeForId<IN_CLIENT_CAME> event_body;
            if (is >> event_body) {
                dest = Event::Create<IN_CLIENT_CAME>(time, event_body);
            }
            break;
        }
        case IN_CLIENT_WAIT: {
            BodyTypeForId<IN_CLIENT_WAIT> event_body;
            if (is >> event_body) {
                dest = Event::Create<IN_CLIENT_WAIT>(time, event_body);
            }
            break;
        }
        case IN_CLIENT_START: {
            BodyTypeForId<IN_CLIENT_START> event_body;
            if (is >> event_body) {
                dest = Event::Create<IN_CLIENT_START>(time, event_body);
            }
            break;
        }
        case IN_CLIENT_GONE: {
            BodyTypeForId<IN_CLIENT_GONE> event_body;
            if (is >> event_body) {
                dest = Event::Create<IN_CLIENT_GONE>(time, event_body);
            }
            break;
        }
        case OUT_CLIENT_GONE: {
            BodyTypeForId<OUT_CLIENT_GONE> event_body;
            if (is >> event_body) {
                dest = Event::Create<OUT_CLIENT_GONE>(time, event_body);
            }
            break;
        }
        case OUT_CLIENT_START: {
            BodyTypeForId<OUT_CLIENT_START> event_body;
            if (is >> event_body) {
                dest = Event::Create<OUT_CLIENT_START>(time, event_body);
            }
            break;
        }
        case OUT_ERROR: {
            BodyTypeForId<OUT_ERROR> event_body;
            if (is >> event_body) {
                dest = Event::Create<OUT_ERROR>(time, event_body);
            }
            break;
        }
        case UNKNOWN: {
            dest = Event();
        }
    }

    return is;
}
