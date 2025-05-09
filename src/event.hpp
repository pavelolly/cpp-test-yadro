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

namespace internal {

class SerializableBody {
public:
    SerializableBody() = default;

    template <typename T>
    SerializableBody(T &&body)
        : body_(std::make_unique<Body<T>>(std::forward<T>(body)))
    {}

    template <typename T>
    const T &Get() const {
        return static_cast<const Body<T> &>(*body_).Get();
    }
    template <typename T>
    T &Get() {
        return static_cast<Body<T> &>(*body_).Get();
    }

    // FIXME: introduce ClonableUniquePtr
    SerializableBody(const SerializableBody &other);
    SerializableBody(SerializableBody &&) = default;
    SerializableBody &operator =(const SerializableBody &other);
    SerializableBody &operator =(SerializableBody &&other) = default;

    bool Empty() const;
    void Dump(std::ostream &os) const;
private:
    struct IBody {
        virtual void Dump(std::ostream &os) const = 0;

        virtual std::unique_ptr<IBody> Clone() const = 0;

        virtual ~IBody() = default;
    };

    template <typename T>
    class Body : public IBody {
    public:
        Body(const T& obj)
            : obj_(obj)
        {}
        Body(T &&obj)
            : obj_(std::move(obj))
        {}

        void Dump(std::ostream &os) const override {
            ::Dump(os, obj_);
        }

        T &Get() {
            return obj_;
        }
        const T &Get() const {
            return obj_;
        }

        std::unique_ptr<IBody> Clone() const override {
            return std::make_unique<Body>(obj_);
        }
    private:
        T obj_;
    };

    std::unique_ptr<IBody> body_;
};

} // namespace internal

class Event {
public:
    Event() = default;
    Event(const Event &) = default;
    Event(Event &&) = default;
    Event &operator=(const Event &) = default;
    Event &operator=(Event &&) = default;

    template <EventId Id>
    static Event Create(TimeStamp time, BodyTypeForId<Id> body) {
        return Event(Id, time, std::move(body));
    }

    EventId GetId() const {
        return id_;
    }
    TimeStamp GetTime() const {
        return time_;
    }

    bool HasBody() const {
        return !body_.Empty();
    }

    template <typename BodyType>
    const BodyType &GetBody() const {
        return body_.Get<BodyType>();
    }
    template <typename BodyType>
    BodyType &GetBody() {
        return body_.Get<BodyType>();
    }
private:
    template <typename BodyType>
    Event(EventId id, TimeStamp time, BodyType &&body)
        : id_(id),
          time_(time),
          body_(std::forward<BodyType>(body))
    {}

    friend void Dump(std::ostream &os, const Event &src);

    EventId id_;
    TimeStamp time_;
    internal::SerializableBody body_;
};

void Dump(std::ostream &os, const Event &src);
std::istream &Load(std::istream &os, Event &dest);
