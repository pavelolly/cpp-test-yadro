#include <gtest/gtest.h>
#include <iostream>

#include "utils/timestamp.cpp"
#include "utils/stream_operators.hpp"
#include "utils/movable_il.hpp"

#include "event.hpp"
#include "reader.hpp"

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

TEST(Load, Event) {

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

    event = Event::Create<EventId::IN_CLIENT_WAIT>(TimeStamp(2, 4), ClientInfo("cli2ent-"));
    LoadAndCompare("02:04 3 cli2ent-", event);

    event = Event::Create<EventId::IN_CLIENT_GONE>(TimeStamp(0, 0), ClientInfo("2clie_0nt"));
    LoadAndCompare("00:00 4 2clie_0nt", event);


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
    ExpectFail("12:34 1 $cl^ient");
    ExpectFail("01:02 1 #clie0nt");
    ExpectFail("01:02 1 =");

    // invalid body
    ExpectFail("12:34 1");
    ExpectFail("02:34 2 client");
    ExpectFail("02:04 3 ");
    ExpectFail("02:04 4 ");

#undef LoadAndCompare
#undef ExpectFail
}

TEST(Load, InputData) {

#define LoadAndCompare(src, data) \
    do { \
        std::istringstream ss(src);             \
        try {                                   \
            InputData dest = LoadInputData(ss); \
            EXPECT_EQ((data), dest);            \
        } catch(InputDataFormatError &e) {  \
            std::cout << e.what() << "\n";  \
            FAIL();                         \
        }                                   \
    } while(0)

    InputData data {
        { 3, TimeStamp(12, 34), TimeStamp(23, 43), 100 },
        {}
    };
    LoadAndCompare(
        "3\n"
        "12:34 23:43\n"
        "100\n",
        data);
    LoadAndCompare(
        "3\n"
        "12:34 23:43\n"
        "100",
        data);

    /*
        apparently you can't init vector of events with initializer list
    
        events = std::vector {
            Event::Create<EventId::IN_CLIENT_CAME>(TimeStamp(1, 2), ClientInfo("client")),
            ...
        };

        actually you can't use initializer_list with non-copyable objects
        you can't even do this

        std::vector<std::unique_ptr<int>> s = {
            nullptr, ...
        };

        which is weird honestly...
    */

    data = InputData {
        { 3, TimeStamp(12, 34), TimeStamp(23, 43), 100 },
        FromIl<std::vector<Event>>(
            {
                Event::Create<EventId::IN_CLIENT_CAME>(TimeStamp(1, 2), ClientInfo("client"))
            }
        )
    };
    LoadAndCompare(
        "3\n"
        "12:34 23:43\n"
        "100\n"
        "01:02 1\t client",
        data);

    data = InputData {
        { 3, TimeStamp(12, 34), TimeStamp(23, 43), 100 },
        FromIl<std::vector<Event>>(
            {
                Event::Create<EventId::IN_CLIENT_CAME>(TimeStamp(1, 2), ClientInfo("client")),
                Event::Create<EventId::IN_CLIENT_START>(TimeStamp(1, 11), ClientTable("client_2", 1)),
                Event::Create<EventId::IN_CLIENT_WAIT>(TimeStamp(2, 56), ClientInfo("client_3")),
                Event::Create<EventId::IN_CLIENT_GONE>(TimeStamp(11, 56), ClientInfo("client_2-"))
            }
        )
    };
    LoadAndCompare(
        "3\n"
        "12:34 23:43\n"
        "100\n"
        "01:02 1 client\n"
        "01:11 2 client_2 1   \n"
        "02:56 3     \vclient_3\n"
        "11:56 4\t client_2-\n",
        data);

    data = InputData {
        { 3, TimeStamp(9, 0), TimeStamp(19, 0), 10 },
        FromIl<std::vector<Event>>(
            {
                Event::Create<EventId::IN_CLIENT_CAME>(TimeStamp(8, 48), ClientInfo("client1")),
                Event::Create<EventId::IN_CLIENT_CAME>(TimeStamp(9, 41), ClientInfo("client1")),
                Event::Create<EventId::IN_CLIENT_CAME>(TimeStamp(9, 48), ClientInfo("client2")),

                Event::Create<EventId::IN_CLIENT_WAIT>(TimeStamp(9, 52), ClientInfo("client1")),

                Event::Create<EventId::IN_CLIENT_START>(TimeStamp(9, 54), ClientTable("client1", 1)),
                Event::Create<EventId::IN_CLIENT_START>(TimeStamp(10, 25), ClientTable("client2", 2)),

                Event::Create<EventId::IN_CLIENT_CAME>(TimeStamp(10, 58), ClientInfo("client3")),
                
                Event::Create<EventId::IN_CLIENT_START>(TimeStamp(10, 59), ClientTable("client3", 3)),

                Event::Create<EventId::IN_CLIENT_CAME>(TimeStamp(11, 30), ClientInfo("client4")),
                
                Event::Create<EventId::IN_CLIENT_START>(TimeStamp(11, 35), ClientTable("client4", 2)),
                
                Event::Create<EventId::IN_CLIENT_WAIT>(TimeStamp(11, 45), ClientInfo("client4")),
                
                Event::Create<EventId::IN_CLIENT_GONE>(TimeStamp(12, 33), ClientInfo("client1")),
                Event::Create<EventId::IN_CLIENT_GONE>(TimeStamp(12, 43), ClientInfo("client2")),
                Event::Create<EventId::IN_CLIENT_GONE>(TimeStamp(15, 52), ClientInfo("client4"))
            }
        )
    };
    LoadAndCompare(
        "3\n"
        "09:00 19:00\n"
        "10\n"
        "08:48 1 client1\n"
        "09:41 1 client1\n"
        "09:48 1 client2\n"
        "09:52 3 client1\n"
        "09:54 2 client1 1\n"
        "10:25 2 client2 2\n"
        "10:58 1 client3\n"
        "10:59 2 client3 3\n"
        "11:30 1 client4\n"
        "11:35 2 client4 2\n"
        "11:45 3 client4\n"
        "12:33 4 client1\n"
        "12:43 4 client2\n"
        "15:52 4 client4\n",
        data);

#undef LoadAndCompare
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}