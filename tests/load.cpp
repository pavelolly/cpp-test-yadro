#include <gtest/gtest.h>
#include <iostream>

#include "utils/timestamp.cpp"
#include "utils/stream_operators.hpp"
#include "utils/movable_il.hpp"
#include "utils/string.hpp"

#include "event.hpp"
#include "club_info_analyzer.hpp"

TEST(Load, TimeStamp) {

#define LoadAndCompare(src, time) \
    do { \
        std::istringstream ss(src);   \
        TimeStamp dest;               \
        ss >> dest;                   \
        EXPECT_TRUE(static_cast<bool>(ss)); \
        if (ss) {                     \
            EXPECT_EQ(dest, (time));  \
        }                             \
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
        EXPECT_TRUE(static_cast<bool>(ss)); \
        if (ss) {                        \
            std::ostringstream ss1, ss2; \
            ss1 << dest;                 \
            ss2 << (event);              \
            EXPECT_EQ(ss1.view(), ss2.view()); \
        }     \
    } while (0)

    auto event = Event::Create<EventId::IN_CLIENT_CAME>(TimeStamp(12, 34), { "client" });
    LoadAndCompare("12:34 1 client", event);

    event = Event::Create<EventId::IN_CLIENT_START>(TimeStamp(2, 34), { "client2", 1 });
    LoadAndCompare("02:34 2 client2 1", event);

    event = Event::Create<EventId::IN_CLIENT_WAIT>(TimeStamp(2, 4), { "cli2ent-" });
    LoadAndCompare("02:04 3 cli2ent-", event);

    event = Event::Create<EventId::IN_CLIENT_GONE>(TimeStamp(0, 0), { "2clie_0nt" });
    LoadAndCompare("00:00 4 2clie_0nt", event);

    event = Event::Create<EventId::IN_CLIENT_GONE>(TimeStamp(0, 0), { "2clie0nt" });
    LoadAndCompare("00:00 4 2clie0nt 1", event); // ok: " 1" stays in stream

    event = Event::Create<EventId::IN_CLIENT_GONE>(TimeStamp(0, 0), { "2clie0nt" });
    LoadAndCompare("00:00 4 2clie0nt 2 3", event); // ok: " 2 3" stays in stream

    event = Event::Create<EventId::OUT_CLIENT_GONE>(TimeStamp(1, 2), { "wahteber" });
    LoadAndCompare("01:02 11 wahteber ", event);

    event = Event::Create<EventId::OUT_CLIENT_START>(TimeStamp(1, 2), { "wahteber", 12 });
    LoadAndCompare("01:02 12 wahteber 12 ", event);

    event = Event::Create<EventId::OUT_ERROR>(TimeStamp(1, 2), { error::YOU_SHALL_NOT_PASS });
    LoadAndCompare("01:02 13 YouShallNotPass", event);

    event = Event::Create<EventId::OUT_ERROR>(TimeStamp(1, 2), { error::NOT_OPEN_YET });
    LoadAndCompare("01:02 13 NotOpenYet", event);

    event = Event::Create<EventId::OUT_ERROR>(TimeStamp(1, 2), { error::PLACE_IS_BUSY });
    LoadAndCompare("01:02 13 PlaceIsBusy", event);

    event = Event::Create<EventId::OUT_ERROR>(TimeStamp(1, 2), { error::CLIENT_UNKNOWN });
    LoadAndCompare("01:02 13 ClientUnknown", event);

    event = Event::Create<EventId::OUT_ERROR>(TimeStamp(1, 2), { error::CAN_WAIT_NO_LONGER });
    LoadAndCompare("01:02 13 ICanWaitNoLonger!", event);

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

    // invalid id
    ExpectFail("12:34 5 client");
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

    // bad error
    ExpectFail("12:34 13 whatever-that-is-not-an-error");
    ExpectFail("12:34 13 $$#%@");

#undef LoadAndCompare
#undef ExpectFail
}


TEST(Load, InputData_Success) {

#define LoadAndCompare(src, data) \
    do { \
        std::istringstream ss(src);             \
        try {                                   \
            InputData dest = LoadInputData(ss); \
            EXPECT_EQ(dest.ntables, (data).ntables);             \
            EXPECT_EQ(dest.time_open, (data).time_open);         \
            EXPECT_EQ(dest.time_close, (data).time_close);       \
            EXPECT_EQ(dest.cost_per_hour, (data).cost_per_hour); \
            EXPECT_EQ(dest.events.size(), (data).events.size()); \
            if (dest.events.size() == (data).events.size()) {    \
                for (int i = 0; i < dest.events.size(); ++i) {   \
                    std::ostringstream ss1, ss2;                 \
                    ss1 << dest.events[i];                       \
                    ss2 << (data).events[i];                     \
                    EXPECT_EQ(ss1.view(), ss2.view());           \
                } \
            }     \
        } catch(InputDataFormatError &e) {  \
            std::cout << e.what() << "\n";  \
            FAIL();                         \
        }                                   \
    } while(0)

    InputData data {
        3, TimeStamp(12, 34), TimeStamp(23, 43), 100,
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

        UPD: you can do it now, as Event is now copyable. but the issue is still there

        actually you can't use initializer_list with non-copyable objects
        you can't even do this

        std::vector<std::unique_ptr<int>> s = {
            nullptr, ...
        };

        which is weird honestly...
    */

    data = InputData {
        3, TimeStamp(12, 34), TimeStamp(23, 43), 100,
        {
            Event::Create<EventId::IN_CLIENT_CAME>(TimeStamp(1, 2), { "client" })
        }
    };
    LoadAndCompare(
        "3\n"
        "12:34 23:43\n"
        "100\n"
        "01:02 1\t client",
        data);

    data = InputData {
        3, TimeStamp(12, 34), TimeStamp(23, 43), 100,
        {
            Event::Create<EventId::IN_CLIENT_CAME> (TimeStamp(1, 2),   { "client" }),
            Event::Create<EventId::IN_CLIENT_START>(TimeStamp(1, 11),  { "client_2", 1 }),
            Event::Create<EventId::IN_CLIENT_WAIT> (TimeStamp(2, 56),  { "client_3" }),
            Event::Create<EventId::IN_CLIENT_GONE> (TimeStamp(11, 56), { "client_2-" })
        }
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
        3, TimeStamp(9, 0), TimeStamp(19, 0), 10,
        {
            Event::Create<EventId::IN_CLIENT_CAME>(TimeStamp(8, 48), { "client1" }),
            Event::Create<EventId::IN_CLIENT_CAME>(TimeStamp(9, 41), { "client1" }),
            Event::Create<EventId::IN_CLIENT_CAME>(TimeStamp(9, 48), { "client2" }),

            Event::Create<EventId::IN_CLIENT_WAIT>(TimeStamp(9, 52), { "client1" }),

            Event::Create<EventId::IN_CLIENT_START>(TimeStamp(9, 54), { "client1", 1 }),
            Event::Create<EventId::IN_CLIENT_START>(TimeStamp(10, 25), { "client2", 2 }),

            Event::Create<EventId::IN_CLIENT_CAME>(TimeStamp(10, 58), { "client3" }),
            
            Event::Create<EventId::IN_CLIENT_START>(TimeStamp(10, 59), { "client3", 3 }),

            Event::Create<EventId::IN_CLIENT_CAME>(TimeStamp(11, 30), { "client4" }),
            
            Event::Create<EventId::IN_CLIENT_START>(TimeStamp(11, 35), { "client4", 2 }),
            
            Event::Create<EventId::IN_CLIENT_WAIT>(TimeStamp(11, 45), { "client4" }),
            
            Event::Create<EventId::IN_CLIENT_GONE>(TimeStamp(12, 33), { "client1" }),
            Event::Create<EventId::IN_CLIENT_GONE>(TimeStamp(12, 43), { "client2" }),
            Event::Create<EventId::IN_CLIENT_GONE>(TimeStamp(15, 52), { "client4" })
        }
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


#define ExpectFail(src, line) \
    do { \
        std::vector<std::string> lines;     \
        lines.reserve(10);                  \
        Split((src), '\n', std::back_inserter(lines));                \
        InputDataFormatError expected_error((line), lines[line - 1]); \
        try { \
            std::istringstream ss(src);         \
            InputData dest = LoadInputData(ss); \
            FAIL();                             \
        } catch (InputDataFormatError &e) {     \
            EXPECT_EQ(std::string(e.what()),               \
                      std::string(expected_error.what())); \
        } \
    } while (0)

TEST(Load, InputData_BadClubInfo) {

#define ExpectFailAbsence(src, line) \
    do { \
        InputDataFormatError expected_error((line)); \
        try { \
            std::istringstream ss(src);         \
            InputData dest = LoadInputData(ss); \
            FAIL();                             \
        } catch (InputDataFormatError &e) {     \
            EXPECT_EQ(std::string(e.what()),               \
                      std::string(expected_error.what())); \
        } \
    } while (0)

    // absence of line 1
    ExpectFailAbsence("", 1);
    ExpectFail("\n\n\n\n", 1);

    // invalid line 1: ntables
    ExpectFail("s",      1);
    ExpectFail("1s\n",   1);
    ExpectFail("1  s\t", 1);
    ExpectFail("-1", 1);
    ExpectFail("1.0", 1);

    // absence of line 2
    ExpectFailAbsence("100", 2);
    ExpectFailAbsence("100\n", 2);
    
    // invalid line 2: timestamps
    ExpectFail(
        "100\n"
        "1:2", 2);
    ExpectFail(
        "100\n"
        "10:24\n", 2);
    ExpectFail(
        "100\n"
        "10:24 s", 2);
    ExpectFail(
        "100\n"
        "10:24 1:2", 2);
    ExpectFail(
        "100\n"
        "10:24 10:28 asd", 2);
    // time open >= timne close
    ExpectFail(
        "100\n"
        "10:28 10:24", 2);
    ExpectFail(
        "100\n"
        "10:24 10:24", 2);

    // absence of line 3
    ExpectFailAbsence(
        "100\n"
        "10:24 10:28", 3);
    ExpectFailAbsence(
        "100\n"
        "10:24 10:28\n", 3);
    
    // invalid line 3: cost_per_hour
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "s", 3);
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "-1", 3);
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "1.0", 3);
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100 4", 3);

#undef ExpectFailAbsence
}


TEST(Load, InputData_BadEvents_Syntax) {
    // time
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "1:2 1 client", 4);
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "s 1 client", 4);
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "25:87 1 client", 4);

    // id
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 j client", 4);
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 1j client", 4);
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 0 client", 4);
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 11 client", 4);
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 123 client", 4);

    // body
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 1", 4);
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 2 client", 4);
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 3 client   1", 4);
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 4 clie+nt", 4);

    // event in the middle
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 4 client\n"
        "10:28 1 client1\n"
        "10:29 2 client#1\n"
        "11:28 1 client1\n", 6);
}


TEST(Load, InputData_BadEvents_Logic) {
    // wrong time sequence
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 4 client\n"
        "10:23 4 client\n", 5);
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 4 client\n"
        "01:02 4 client\n", 5);

    // wrong table_number
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 2 client -1\n", 4);
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 2 client 0\n", 4);
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 2 client 1000\n", 4);
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 2 client s\n", 4);

    // output event
    ExpectFail(
        "100\n"
        "10:24 10:28\n"
        "100\n"
        "10:24 11 client\n", 4);
}

#undef ExpectFail


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}