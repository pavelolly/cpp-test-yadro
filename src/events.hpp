#include <ctime>
#include <memory>
#include <string>
#include <string_view>

namespace event {

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

struct EventBody {
    virtual ~EventBody() = 0;
};

struct BodyClientInfo : EventBody {
    std::string client_name;
};

struct BodyClientTable : EventBody {
    std::string client_name;
    int table_number;
};

struct BodyError : EventBody {
    std::string_view message;
};

template <EventId Id>
struct BodyFor { using type = BodyClientInfo; };
template<> struct BodyFor<EventId::IN_CLIENT_START>  { using type = BodyClientTable; };
template<> struct BodyFor<EventId::OUT_CLIENT_START> { using type = BodyClientTable; };
template<> struct BodyFor<EventId::OUT_ERROR>        { using type = BodyError; };

class Event {

    template <EventId Id, typename BodyType = typename BodyFor<Id>::type>
    static Event Create(std::tm time, BodyType body) {
        return Event(id, time, std::make_unique<BodyType>(std::move(body)));
    }

private:
    Event(EventId id, std::tm time, std::unique_ptr<EventBody> body = nullptr)
        : id_(id), time_(time), body_(std::move(body)) {}
    
    EventId id_;
    std::tm time_;
    std::unique_ptr<EventBody> body_;
};

} // namespace events