#pragma once

#include "body.hpp"

enum class EventId {
    UNKNOWN = 0,
    // Input Events
    IN_CLIENT_CAME = 1,
    IN_CLIENT_START,
    IN_CLIENT_WAIT,
    IN_CLIENT_GONE,

    // Output Events
    OUT_CLIENT_GONE = 11,
    OUT_CLIENT_START,
    OUT_ERROR
};

inline bool IsInputEventId(int val) {
    using enum EventId;
    return static_cast<int>(IN_CLIENT_CAME) <= val && val <= static_cast<int>(IN_CLIENT_GONE);
}
inline bool IsOutputEventId(int val) {
    using enum EventId;
    return static_cast<int>(OUT_CLIENT_GONE) <= val && val <= static_cast<int>(OUT_ERROR);
}
inline bool IsEventId(int val) {
    return IsInputEventId(val) || IsOutputEventId(val) || val == static_cast<int>(EventId::UNKNOWN);
}

template <EventId Id> struct BodyTypeFor {};
template <> struct BodyTypeFor<EventId::IN_CLIENT_CAME>   { using type = body::ClientInfo; };
template <> struct BodyTypeFor<EventId::IN_CLIENT_GONE>   { using type = body::ClientInfo; };
template <> struct BodyTypeFor<EventId::IN_CLIENT_START>  { using type = body::ClientTable; };
template <> struct BodyTypeFor<EventId::IN_CLIENT_WAIT>   { using type = body::ClientInfo; };
template <> struct BodyTypeFor<EventId::OUT_CLIENT_GONE>  { using type = body::ClientInfo; };
template <> struct BodyTypeFor<EventId::OUT_CLIENT_START> { using type = body::ClientTable; };
template <> struct BodyTypeFor<EventId::OUT_ERROR>        { using type = body::Error; };

template <EventId Id>
using BodyTypeForId = typename BodyTypeFor<Id>::type;
