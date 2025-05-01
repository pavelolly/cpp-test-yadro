#include <chrono>
#include <memory>
#include <string>
#include <string_view>
#include <concepts>

namespace events {

struct Body {
    // TODO: this should be external dumper
    virtual void Dump(std::ostream &) const = 0;

    virtual ~Body() = default;
};

struct ClientInfo : Body {
    std::string client_name;

    ClientInfo(std::string client_name)
        : client_name(std::move(client_name)) {}

    void Dump(std::ostream &os) const override {
        os << client_name;
    }
};

struct ClientTable : Body {
    std::string client_name;
    int table_number;

    ClientTable(std::string client_name, int table_number)
        : client_name(std::move(client_name)), table_number(table_number) {}

    void Dump(std::ostream &os) const override {
        os << client_name << " " << table_number;
    }
};

struct Error : Body {
    std::string_view message;

    Error(std::string_view message) : message(message) {}

    void Dump(std::ostream &os) const override {
        os << message;
    }
};

class Event {
public:
    enum class Id {
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

    template <Event::Id Id> struct BodyTypeFor {};
    template <> struct BodyTypeFor<Event::Id::IN_CLIENT_CAME>   { using type = ClientInfo; };
    template <> struct BodyTypeFor<Event::Id::IN_CLIENT_START>  { using type = ClientTable; };
    template <> struct BodyTypeFor<Event::Id::IN_CLIENT_WAIT>   { using type = ClientInfo; };
    template <> struct BodyTypeFor<Event::Id::IN_CLIENT_GONE>   { using type = ClientInfo; };

    template <> struct BodyTypeFor<Event::Id::OUT_CLIENT_GONE>  { using type = ClientInfo; };
    template <> struct BodyTypeFor<Event::Id::OUT_CLIENT_START> { using type = ClientTable; };
    template <> struct BodyTypeFor<Event::Id::OUT_ERROR>        { using type = Error; };

    template <Event::Id Id>
    using BodyTypeForId = typename BodyTypeFor<Id>::type;

    using TimeStamp = std::chrono::minutes;

    template <Event::Id Id>
    static Event Create(TimeStamp time, BodyTypeForId<Id> body) {
        return Event(Id, time, std::make_unique<BodyTypeForId<Id>>(std::move(body)));
    }

    Id GetId()        const { return id_; }
    TimeStamp GetTime() const { return time_; }

    Body &GetBody() { return *body_; }
    const Body &GetBody() const { return *body_; }

    template <typename BodyType>
        requires std::is_base_of<Body, BodyType>
    BodyType &GetBody() { return static_cast<BodyType &>(*body_); }

    template <typename BodyType>
        requires std::is_base_of<Body, BodyType>
    const BodyType &GetBody() const { return static_cast<const BodyType &>(*body_); }

    bool HasBody() const { return static_cast<bool>(body_); }

    void Dump(std::ostream &os) const {
        os << (time_ / 60).count() << ":" << (time_ % 60).count() << " " << static_cast<int>(id_) << " ";
        body_->Dump(os);
    }
private:
    Event(Id id, TimeStamp time, std::unique_ptr<Body> body = nullptr)
        : id_(id), time_(time), body_(std::move(body)) {}

    Id id_;
    TimeStamp time_;
    std::unique_ptr<Body> body_;
};

} // namespace events