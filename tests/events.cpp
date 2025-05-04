#include <gtest/gtest.h>
#include <iostream>

#include "event.hpp"
#include "utils/stream_operators.hpp"

TEST(Events, Creation) {

#define DumpAndCompare(event, str) \
    do {                           \
        std::ostringstream ss;     \
        ss << (event);               \
        EXPECT_EQ(ss.view(), (str)); \
    } while(0)

    using enum EventId;
    
    TimeStamp time(12, 34);

    auto ev = Event::Create<IN_CLIENT_CAME>(time, body::ClientInfo("client1"));
    DumpAndCompare(ev, "12:34 1 client1");

    ev = Event::Create<IN_CLIENT_START>(time, body::ClientTable("client2", 12));
    DumpAndCompare(ev, "12:34 2 client2 12");

    time = TimeStamp(2, 35);

    ev = Event::Create<IN_CLIENT_WAIT>(time, body::ClientInfo("client3"));
    DumpAndCompare(ev, "02:35 3 client3");

    ev = Event::Create<IN_CLIENT_GONE>(time, body::ClientInfo("client3"));
    DumpAndCompare(ev, "02:35 4 client3");

    time = TimeStamp(23, 1);

    ev = Event::Create<OUT_CLIENT_GONE>(time, body::ClientInfo("client1"));
    DumpAndCompare(ev, "23:01 11 client1");

    ev = Event::Create<OUT_CLIENT_START>(time, body::ClientTable("client2", 14));
    DumpAndCompare(ev, "23:01 12 client2 14");

    time = TimeStamp(2, 2);

    ev = Event::Create<OUT_ERROR>(time, body::Error("message"));
    DumpAndCompare(ev, "02:02 13 message");

    std::cout << "Here to check if standart operator << still works " << 69420 << "\n";

#undef DumpAndComare
}

namespace {

template <typename T, typename = void>
struct exists : std::false_type {};

template <typename T>
struct exists<T, std::void_t<typename T::type>> : std::true_type {};

template <typename T>
inline constexpr bool exists_v = exists<T>::value;

#define ASSERT_EXISTS(type) static_assert(exists_v<type>)
#define ASSERT_NOT_EXISTS(type) static_assert(!exists_v<type>)

} // namespace

TEST(Events, EventIds) {
    using enum EventId;

    ASSERT_NOT_EXISTS(BodyTypeFor<UNKNOWN>);
    ASSERT_EXISTS(BodyTypeFor<IN_CLIENT_CAME>);
    ASSERT_EXISTS(BodyTypeFor<IN_CLIENT_START>);
    ASSERT_EXISTS(BodyTypeFor<IN_CLIENT_WAIT>);
    ASSERT_EXISTS(BodyTypeFor<IN_CLIENT_GONE>);
    ASSERT_EXISTS(BodyTypeFor<OUT_CLIENT_GONE>);
    ASSERT_EXISTS(BodyTypeFor<OUT_CLIENT_START>);
    ASSERT_EXISTS(BodyTypeFor<OUT_ERROR>);

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
            case UNKNOWN:
                EXPECT_TRUE(IsEventId(static_cast<int>(id)));
                break;
        }
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}