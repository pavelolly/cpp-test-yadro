#pragma once

#include "body.hpp"

enum class EventId {
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

template <EventId Id> struct BodyTypeFor {};
template <> struct BodyTypeFor<EventId::IN_CLIENT_CAME>   { using type = ClientInfo; };
template <> struct BodyTypeFor<EventId::IN_CLIENT_START>  { using type = ClientTable; };
template <> struct BodyTypeFor<EventId::IN_CLIENT_WAIT>   { using type = ClientInfo; };
template <> struct BodyTypeFor<EventId::IN_CLIENT_GONE>   { using type = ClientInfo; };
template <> struct BodyTypeFor<EventId::OUT_CLIENT_GONE>  { using type = ClientInfo; };
template <> struct BodyTypeFor<EventId::OUT_CLIENT_START> { using type = ClientTable; };
template <> struct BodyTypeFor<EventId::OUT_ERROR>        { using type = Error; };

template <EventId Id>
using BodyTypeForId = typename BodyTypeFor<Id>::type;