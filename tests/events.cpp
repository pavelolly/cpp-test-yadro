#include <gtest/gtest.h>
#include <iostream>

#include "event.hpp"

TEST(Events, Creation) {
    auto DumpAndCompare = [](const Event &event, std::string_view str) {
        std::ostringstream ss;
        ss << event;
        EXPECT_EQ(ss.view(), str);
    };

    using enum EventId;
    
    TimeStamp time(12, 34);

    auto ev = Event::Create<IN_CLIENT_CAME>(time, ClientInfo("client1"));
    DumpAndCompare(ev, "12:34 1 client1");

    ev = Event::Create<IN_CLIENT_START>(time, ClientTable("client2", 12));
    DumpAndCompare(ev, "12:34 2 client2 12");

    ev = Event::Create<IN_CLIENT_WAIT>(time, ClientInfo("client3"));
    DumpAndCompare(ev, "12:34 3 client3");

    ev = Event::Create<IN_CLIENT_GONE>(time, ClientInfo("client3"));
    DumpAndCompare(ev, "12:34 4 client3");

    ev = Event::Create<OUT_CLIENT_GONE>(time, ClientInfo("client1"));
    DumpAndCompare(ev, "12:34 11 client1");

    ev = Event::Create<OUT_CLIENT_START>(time, ClientTable("client2", 14));
    DumpAndCompare(ev, "12:34 12 client2 14");

    ev = Event::Create<OUT_ERROR>(time, Error("message"));
    DumpAndCompare(ev, "12:34 13 message");

    std::cout << "Here to check if standart operator << still works " << 69420 << "\n";
}

namespace {

template <typename T, typename = void>
struct exists : std::false_type {};

template <typename T>
struct exists<T, std::void_t<T>> : std::true_type {};

template <typename T>
inline constexpr bool exists_v = exists<T>::value;

} // namespace

TEST(Events, EventIds) {
    using enum EventId;

    EventId all_ids[] = {
        IN_CLIENT_CAME,
        IN_CLIENT_START,
        IN_CLIENT_WAIT,
        IN_CLIENT_GONE,
        OUT_CLIENT_GONE,
        OUT_CLIENT_START,
        OUT_ERROR
    };

    for (auto id : all_ids) {
        switch (id) {
            case IN_CLIENT_CAME:
            case IN_CLIENT_START:
            case IN_CLIENT_WAIT:
            case IN_CLIENT_GONE:
                EXPECT_TRUE(IsEventId(static_cast<int>(id)));
                EXPECT_FALSE(IsOutputEventId(static_cast<int>(id)));
                EXPECT_TRUE(IsInputEventId(static_cast<int>(id)));
                break;
            case OUT_CLIENT_GONE:
            case OUT_CLIENT_START:
            case OUT_ERROR:
                EXPECT_TRUE(IsEventId(static_cast<int>(id)));
                EXPECT_TRUE(IsOutputEventId(static_cast<int>(id)));
                EXPECT_FALSE(IsInputEventId(static_cast<int>(id)));
                break;
        }
    }

#define EXISTS(type) static_assert(exists_v<type>)

    EXISTS(BodyTypeForId<IN_CLIENT_CAME>);
    EXISTS(BodyTypeForId<IN_CLIENT_START>);
    EXISTS(BodyTypeForId<IN_CLIENT_WAIT>);
    EXISTS(BodyTypeForId<IN_CLIENT_GONE>);
    EXISTS(BodyTypeForId<OUT_CLIENT_GONE>);
    EXISTS(BodyTypeForId<OUT_CLIENT_START>);
    EXISTS(BodyTypeForId<OUT_ERROR>);

#undef EXISTS
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}