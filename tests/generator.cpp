#include <iostream>
#include <array>
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
bool Chance(float prob) {
    static std::uniform_real_distribution<float> rand;
    return rand(gen) < prob;
}

// input data must have well defined metadata (ntables, timestamps, ...)
void GenerateRandomEvents(InputData &data, int nevents) {
    std::vector<Event> events;

    std::uniform_int_distribution<> rand_id(
        static_cast<int>(EventId::IN_CLIENT_CAME),
        static_cast<int>(EventId::IN_CLIENT_GONE)
    );

    std::uniform_int_distribution<> rand_client(0, std::size(CLIENT_NAMES) - 1);
    std::uniform_int_distribution<> rand_table(1, data.ntables);

    // TODO: check edge cases when time is less 0:0 or bigger 24:00
    TimeStamp time = data.time_open - TimeStamp(1, 0);
    TimeStamp step = (data.time_close - data.time_open + TimeStamp(2, 0)) / nevents;
    std::chrono::minutes variance = step / 4;

    std::uniform_int_distribution<> rand_minutes(-variance.count(), variance.count());

    for (int i = 0; i < nevents; ++i) {
        EventId id = static_cast<EventId>(rand_id(gen));
        std::string_view client = CLIENT_NAMES[rand_client(gen)];

        switch (id) {
            using enum EventId;

            case IN_CLIENT_CAME:
                events.push_back(Event::Create<IN_CLIENT_CAME>(time, { std::string(client) } ));
                break;
            case IN_CLIENT_START: {
                // table is invalid with 15% chance
                // int table = Chance(0.85) ? rand_table(gen) : rand_table(gen) + data.ntables;
                events.push_back(Event::Create<IN_CLIENT_START>(time, { std::string(client), rand_table(gen) }));
                break;
            }
            case IN_CLIENT_WAIT:
                events.push_back(Event::Create<IN_CLIENT_WAIT>(time, { std::string(client) } ));
                break;
            case IN_CLIENT_GONE:
                events.push_back(Event::Create<IN_CLIENT_GONE>(time, { std::string(client) } ));
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
        10, TimeStamp(8, 0), TimeStamp(22, 0), 100, {}
    };

    GenerateRandomEvents(data, 25);

    for (auto &event : data.events) {
        std::cout << event << "\n";
    }

    return 0;
}