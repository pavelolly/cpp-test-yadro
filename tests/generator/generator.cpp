#include <iostream>
#include <unordered_set>
#include <random>
#include <cassert>

#include "analyzer.hpp"
#include "utils/stream_operators.hpp"

std::string_view CLIENT_NAMES[] {
    "shepard",
    "ashley",
    "kaidan",
    "garrus",
    "liara",
    "tali",
    "wrex",
    "jeff",
    "kasumi",
    "miranda",
    "thane",
    "edi",
    "kelly",
    "samantha"
};

std::mt19937 gen(std::random_device{}());

// true with probability
bool Chance(double prob) {
    static std::uniform_real_distribution rand;
    return rand(gen) < prob;
}

struct CommonBody {
    EventId id;
    std::string_view client_name;
};

// input data must have well defined metadata (ntables, timestamps, ...)
void GenerateRandomEvents(InputData &data, int nevents) {
    std::uniform_int_distribution rand_client(0, (int)std::size(CLIENT_NAMES) - 1);
    std::uniform_int_distribution rand_table(1, data.ntables);
    
    std::unordered_set<std::string_view> clients_in_club;

    // TODO: check edge cases when time is less 0:0 or bigger 24:00
    TimeStamp time = data.time_open - TimeStamp(1, 0);
    TimeStamp step = (data.time_close - data.time_open + TimeStamp(2, 0)) / nevents;
    std::chrono::minutes variance = step / 4;

    std::uniform_int_distribution rand_minutes(-variance.count(), variance.count());

    auto GetIdAndName = [&]() -> CommonBody {
        // 10% chance to generate completely random event
        if (Chance(0.1)) {
            std::uniform_int_distribution rand_id(
                static_cast<int>(EventId::IN_CLIENT_CAME),
                static_cast<int>(EventId::IN_CLIENT_GONE)
            );

            return { static_cast<EventId>(rand_id(gen)), CLIENT_NAMES[rand_client(gen)] };
        }

        // if nobody is in the club or with 25% chance someone must come
        if (clients_in_club.empty() || Chance(0.25)) {
            auto client = CLIENT_NAMES[rand_client(gen)];
            if (time >= data.time_open) {
                clients_in_club.insert(client);
            }

            return { EventId::IN_CLIENT_CAME, client };
        }

        // otherwise pick somebody in the club and generete event for them
        std::uniform_int_distribution rand(0, (int)clients_in_club.size() - 1);

        EventId id;
        if (Chance(0.4)) {
            id = EventId::IN_CLIENT_START;
        } else {
            std::uniform_int_distribution rand_id(
                static_cast<int>(EventId::IN_CLIENT_WAIT),
                static_cast<int>(EventId::IN_CLIENT_GONE)
            );
            id = static_cast<EventId>(rand_id(gen));
        }

        return { id, *std::next(clients_in_club.begin(), rand(gen)) };
    };

    std::vector<Event> events;

    for (int i = 0; i < nevents; ++i) {
        auto [id, client] = GetIdAndName();

        switch (id) {
            using enum EventId;

            case IN_CLIENT_CAME:
                events.push_back(Event::Create<IN_CLIENT_CAME>(time, { std::string(client) } ));
                break;
            case IN_CLIENT_START: {
                events.push_back(Event::Create<IN_CLIENT_START>(time, { std::string(client), rand_table(gen) }));
                break;
            }
            case IN_CLIENT_WAIT:
                events.push_back(Event::Create<IN_CLIENT_WAIT>(time, { std::string(client) } ));
                break;
            case IN_CLIENT_GONE:
                events.push_back(Event::Create<IN_CLIENT_GONE>(time, { std::string(client) } ));
                clients_in_club.erase(client);
                break;
            
            case OUT_CLIENT_GONE:
            case OUT_CLIENT_START:
            case OUT_ERROR:
            case UNKNOWN:
                assert(false && "Invalid id generated");
        }

        time += step + TimeStamp(0, rand_minutes(gen));
    }

    data.events = std::move(events);
}

int main() {
    InputData data = {
        3, TimeStamp(10, 0), TimeStamp(21, 0), 100, {}
    };

    GenerateRandomEvents(data, 50);

    for (auto &event : data.events) {
        std::cout << event << "\n";
    }

    return 0;
}
