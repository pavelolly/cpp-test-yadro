#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <concepts>

#include "utils/timestamp.hpp"
#include "event_id.hpp"
#include "body.hpp"

class Event;

void Dump(std::ostream &os, const Event &src);
std::istream &Load(std::istream &os, Event &dest);

// this should be included after all Dump functions declared /* (== *crap*) */
#include "utils/serializer.hpp"

class Event {
public:
    Event() = default;

    template <EventId Id>
    static Event Create(TimeStamp time, BodyTypeForId<Id> body) {
        return Event(Id, time, std::make_unique<BodyTypeForId<Id>>(std::move(body)));
    }

    EventId GetId() const {
        return id_;
    }
    TimeStamp GetTime() const {
        return time_;
    }

    bool HasBody() const {
        return static_cast<bool>(body_);
    }

    template <typename BodyType = Body>
        requires std::is_base_of_v<Body, BodyType>
    BodyType &GetBody() {
        return static_cast<BodyType &>(*body_);
    }

    template <typename BodyType = Body>
        requires std::is_base_of_v<Body, BodyType>
    const BodyType &GetBody() const {
        return static_cast<const BodyType &>(*body_);
    }

    const Serializer &GetBodySerializer() const {
        return body_serializer_;
    }

    bool operator ==(const Event &other) const {
        if (id_ != other.id_) {
            return false;
        }
        if (time_ != other.time_) {
            return false;
        }
        if (HasBody() != other.HasBody()) {
            return false;
        }
        if (!HasBody() && !other.HasBody()) {
            return true;
        }
        if (*body_ != *other.body_) {
            return false;
        }
        return true;        
    }
    bool operator !=(const Event &other) const {
        return !operator ==(other);
    }
private:
    template <typename BodyType = Body>
        requires std::is_base_of_v<Body, BodyType>
    Event(EventId id, TimeStamp time, std::unique_ptr<BodyType> body)
        : id_(id),
          time_(time),
          body_(std::move(body)),
          body_serializer_(static_cast<BodyType &>(*body_))
    {}

    EventId id_;
    TimeStamp time_;
    std::unique_ptr<Body> body_;

    Serializer body_serializer_;
};