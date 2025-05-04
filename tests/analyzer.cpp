#include <gtest/gtest.h>
#include <iostream>
#include <iterator>
#include <algorithm>

#include "club_info_analyzer.hpp"
#include "utils/stream_operators.hpp"

TEST(ProcessInputData, Events) {

#define LoadAndCompare(src, expected_events) \
    do { \
        std::istringstream is(src);         \
        InputData data = LoadInputData(is); \
        OutputData res = ProcessInputData(data);     \
        is = std::istringstream(expected_events);    \
        std::vector<Event> events;                   \
        std::copy(std::istream_iterator<Event>(is),  \
            std::istream_iterator<Event>(),          \
            std::back_inserter(events));             \
        EXPECT_EQ(res.events.size(), events.size()); \
        if (res.events.size() == events.size()) {    \
            for (int i = 0; i < (int)res.events.size(); ++i) { \
                std::ostringstream ss1, ss2;                   \
                ss1 << res.events[i];                          \
                ss2 << events[i];                              \
                EXPECT_EQ(ss1.view(), ss2.view());             \
            } \
        }     \
    } while(0)

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
       "15:52 4 client4\n"
       ,
       "08:48 1 client1\n"
       "08:48 13 NotOpenYet\n"
       "09:41 1 client1\n"
       "09:48 1 client2 \n"
       "09:52 3 client1\n"
       "09:52 13 ICanWaitNoLonger!\n"
       "09:54 2 client1 1\n"
       "10:25 2 client2 2\n"
       "10:58 1 client3 \n"
       "10:59 2 client3 3\n"
       "11:30 1 client4\n"
       "11:35 2 client4 2\n"
       "11:35 13 PlaceIsBusy\n"
       "11:45 3 client4\n"
       "12:33 4 client1\n"
       "12:33 12 client4 1\n"
       "12:43 4 client2\n"
       "15:52 4 client4\n"
       "19:00 11 client3\n"
    );

#undef LoadAndCompareEvents
}

TEST(ProcessInputData, TableInfos) {

    #define LoadAndCompare(src, map) \
        do { \
            std::istringstream is(src);         \
            InputData data = LoadInputData(is); \
            OutputData res = ProcessInputData(data);         \
            EXPECT_EQ(res.table_infos.size(), (map).size()); \
            if (res.table_infos.size() == (map).size()) {    \
                for (int i = 1; i <= res.table_infos.size(); ++i) {              \
                    EXPECT_EQ(res.table_infos[i].earnings, (map)[i].earnings);   \
                    EXPECT_EQ(res.table_infos[i].time_used, (map)[i].time_used); \
                }                                                                \
            } \
        } while (0)

    std::map<int, TableInfo> map {
        {1, {70, TimeStamp(5, 58)}},
        {2, {30, TimeStamp(2, 18)}},
        {3, {90, TimeStamp(8, 1)}},
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
       map
    );
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}