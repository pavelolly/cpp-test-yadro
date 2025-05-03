#include <gtest/gtest.h>
#include <iostream>

#include "utils/timestamp.cpp"
#include "utils/stream_operators.hpp"

#include "event.hpp"

TEST(Load, TimeStamp) {

#define LoadAndCompare(src, time) \
    do { \
        std::istringstream ss(src);   \
        TimeStamp dest;               \
        ss >> dest;                   \
        EXPECT_EQ(dest, (time));      \
    } while(0)

    LoadAndCompare("12:34", TimeStamp(12, 34));
    LoadAndCompare(" 02:34", TimeStamp(2, 34));
    LoadAndCompare("  02:04", TimeStamp(2, 4));
    LoadAndCompare("\t\v22:04", TimeStamp(22, 4));
    LoadAndCompare("\n23:59", TimeStamp(23, 59));

#define ExpectFail(src) \
    do { \
        std::istringstream ss(src); \
        TimeStamp dest;               \
        ss >> dest;                   \
        EXPECT_TRUE(ss.fail());       \
    } while(0)

    ExpectFail("asjdh");
    ExpectFail("123");
    ExpectFail("  24:00");
    ExpectFail(" 2b4:00");
    ExpectFail("12:61");
    ExpectFail("\n12:-61");
    ExpectFail("");
    ExpectFail("\t");
    ExpectFail("12:3");
    ExpectFail("2:13");
    ExpectFail("2:3");
    ExpectFail("1:2:-61");

#undef LoadAndCompare
#undef ExpectFail
}

#define I(...) __VA_ARGS__

TEST(Load, EventBody) {

#define LoadAndCompare(src, event) \
    do { \
        std::istringstream ss(src); \
        Event dest;                 \
        ss >> dest;                 \
        EXPECT_EQ(dest.GetId(),   (event).GetId());   \
        EXPECT_EQ(dest.GetTime(), (event).GetTime()); \
        ASSERT_EQ(dest.HasBody(), (event).HasBody()); \
        if (dest.HasBody()) {                         \
            EXPECT_EQ(dest.GetBody(), (event).GetBody()); \
        } \
    } while (0)

    auto event = Event::Create<EventId::IN_CLIENT_CAME>(TimeStamp(12, 34), ClientInfo("client"));
    LoadAndCompare("12:34 1 client", event);

    event = Event::Create<EventId::IN_CLIENT_START>(TimeStamp(2, 34), ClientTable("client2", 1));
    LoadAndCompare("02:34 2 client2 1", event);

    event = Event::Create<EventId::IN_CLIENT_WAIT>(TimeStamp(2, 4), ClientInfo("cli2ent"));
    LoadAndCompare("02:04 3 cli2ent", event);

    event = Event::Create<EventId::IN_CLIENT_GONE>(TimeStamp(0, 0), ClientInfo("2clie0nt"));
    LoadAndCompare("00:00 4 2clie0nt", event);


    event = Event::Create<EventId::IN_CLIENT_GONE>(TimeStamp(0, 0), ClientInfo("2clie0nt"));
    LoadAndCompare("00:00 4 2clie0nt 1", event); // ok: " 1" stays in stream

    event = Event::Create<EventId::IN_CLIENT_GONE>(TimeStamp(0, 0), ClientInfo("2clie0nt"));
    LoadAndCompare("00:00 4 2clie0nt 2 3", event); // ok: " 2 3" stays in stream

#define ExpectFail(src) \
    do { \
        std::istringstream ss(src); \
        Event dest;                 \
        ss >> dest ;                \
        EXPECT_TRUE(ss.fail());     \
    } while(0)

    // obvious
    ExpectFail("kalsjhd");
    ExpectFail("  asd  wq \b");
    ExpectFail("");
    ExpectFail(" \n\t");

    // invalid id or id of output event
    ExpectFail("12:34 5 client");
    ExpectFail("12:34 11 client");
    ExpectFail("12:34 13 client");
    ExpectFail("12:34 234 client");

    // invalid time
    ExpectFail("1234 1 client");
    ExpectFail("2:13 1 client");
    ExpectFail("1:2:-61 1 client");

    // invalid name
    // ExpectFail("12:34 1 client");
    // ExpectFail("02:13 1 clie_nt");
    // ExpectFail("01:02: 1 #clie0nt");

    // invalid body
    ExpectFail("12:34 1");
    ExpectFail("02:34 2 client");
    ExpectFail("02:04 3 ");
    ExpectFail("02:04 4 ");

#undef LoadAndCompare
#undef ExpectFail
}

#if 0

TEST(Load, Event) {

}

TEST(Load, InputData) {

}

#endif // #if 0

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}