#include "event.hpp"

bool IsInputEventId(int val) {
    using enum EventId;
    return static_cast<int>(IN_CLIENT_CAME) <= val && val <= static_cast<int>(IN_CLIENT_GONE);
}

bool IsOutputEventId(int val) {
    using enum EventId;
    return static_cast<int>(OUT_CLIENT_GONE) <= val && val <= static_cast<int>(OUT_ERROR);
}

bool IsEventId(int val) {
    return IsInputEventId(val) || IsOutputEventId(val);
}

void Dump(std::ostream &os, const Event &event) {
    Dump(os, event.GetTime());
    os << " " << static_cast<int>(event.GetId()) << " ";
    event.GetBodySerializer().Dump(os);
}