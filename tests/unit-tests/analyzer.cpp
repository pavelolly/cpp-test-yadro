#include <gtest/gtest.h>
#include <iostream>
#include <iterator>
#include <algorithm>

#include "analyzer.hpp"
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

    LoadAndCompare(
        "10\n"
        "08:00 22:00\n"
        "100\n"
        "07:00 2 kasumi 10\n"
        "07:45 1 liara\n"
        "08:15 1 jeff\n"
        "08:55 4 kelly\n"
        "09:38 2 samantha 5\n"
        "10:10 2 miranda 8\n"
        "10:48 4 wrex\n"
        "11:23 3 garrus\n"
        "12:00 1 garrus\n"
        "12:31 4 edi\n"
        "13:05 1 samantha\n"
        "13:47 2 edi 4\n"
        "14:24 3 liara\n"
        "14:58 4 jeff\n"
        "15:43 3 miranda\n"
        "16:21 3 kasumi\n"
        "16:52 2 thane 4\n"
        "17:26 4 kelly\n"
        "18:12 2 edi 6\n"
        "18:54 1 liara\n"
        "19:38 1 kelly\n"
        "20:11 1 miranda\n"
        "20:49 1 ashley\n"
        "21:23 4 garrus\n"
        "22:07 4 miranda\n"
        ,
        "07:00 2 kasumi 10\n"
        "07:00 13 ClientUnknown\n"
        "07:45 1 liara\n"
        "07:45 13 NotOpenYet\n"
        "08:15 1 jeff\n"
        "08:55 4 kelly\n"
        "08:55 13 ClientUnknown\n"
        "09:38 2 samantha 5\n"
        "09:38 13 ClientUnknown\n"
        "10:10 2 miranda 8\n"
        "10:10 13 ClientUnknown\n"
        "10:48 4 wrex\n"
        "10:48 13 ClientUnknown\n"
        "11:23 3 garrus\n"
        "11:23 13 ClientUnknown\n"
        "12:00 1 garrus\n"
        "12:31 4 edi\n"
        "12:31 13 ClientUnknown\n"
        "13:05 1 samantha\n"
        "13:47 2 edi 4\n"
        "13:47 13 ClientUnknown\n"
        "14:24 3 liara\n"
        "14:24 13 ClientUnknown\n"
        "14:58 4 jeff\n"
        "15:43 3 miranda\n"
        "15:43 13 ClientUnknown\n"
        "16:21 3 kasumi\n"
        "16:21 13 ClientUnknown\n"
        "16:52 2 thane 4\n"
        "16:52 13 ClientUnknown\n"
        "17:26 4 kelly\n"
        "17:26 13 ClientUnknown\n"
        "18:12 2 edi 6\n"
        "18:12 13 ClientUnknown\n"
        "18:54 1 liara\n"
        "19:38 1 kelly\n"
        "20:11 1 miranda\n"
        "20:49 1 ashley\n"
        "21:23 4 garrus\n"
        "22:07 4 miranda\n"
        "22:07 13 ClientUnknown\n"
        "22:00 11 ashley\n"
        "22:00 11 kelly\n"
        "22:00 11 liara\n"
        "22:00 11 miranda\n"
        "22:00 11 samantha\n"
    );

    LoadAndCompare(
        "5\n"
        "08:00 22:00\n"
        "100\n"
        "07:00 1 thane\n"
        "08:19 1 wrex\n"
        "09:34 3 wrex\n"
        "10:30 1 kaidan\n"
        "11:25 1 garrus\n"
        "12:21 4 kaidan\n"
        "13:21 2 garrus 3\n"
        "14:09 2 garrus 3\n"
        "15:01 2 wrex 4\n"
        "16:19 4 wrex\n"
        "17:35 2 garrus 5\n"
        "18:41 3 garrus\n"
        "20:00 1 ashley\n"
        "21:14 2 garrus 5\n"
        "22:13 1 shepard\n"
        ,
        "07:00 1 thane\n"
        "07:00 13 NotOpenYet\n"
        "08:19 1 wrex\n"
        "09:34 3 wrex\n"
        "09:34 13 ICanWaitNoLonger!\n"
        "10:30 1 kaidan\n"
        "11:25 1 garrus\n"
        "12:21 4 kaidan\n"
        "13:21 2 garrus 3\n"
        "14:09 2 garrus 3\n"
        "15:01 2 wrex 4\n"
        "16:19 4 wrex\n"
        "17:35 2 garrus 5\n"
        "18:41 3 garrus\n"
        "18:41 13 ICanWaitNoLonger!\n"
        "20:00 1 ashley\n"
        "21:14 2 garrus 5\n"
        "22:13 1 shepard\n"
        "22:13 13 NotOpenYet\n"
        "22:00 11 ashley\n"
        "22:00 11 garrus\n"
    );

#undef LoadAndCompare
}

TEST(ProcessInputData, TableInfos) {

    #define LoadAndCompare(src, map) \
        do { \
            std::istringstream is(src);         \
            InputData data = LoadInputData(is); \
            OutputData res = ProcessInputData(data);         \
            EXPECT_EQ(res.table_infos.size(), (map).size()); \
            if (res.table_infos.size() == (map).size()) {    \
                for (int i = 1; i <= (int)res.table_infos.size(); ++i) {         \
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

    map = {
        {1, {0, TimeStamp()}},
        {2, {0, TimeStamp()}},
        {3, {0, TimeStamp()}},
        {4, {0, TimeStamp()}},
        {5, {0, TimeStamp()}},
        {6, {0, TimeStamp()}},
        {7, {0, TimeStamp()}},
        {8, {0, TimeStamp()}},
        {9, {0, TimeStamp()}},
        {10, {0, TimeStamp()}},
    };

    LoadAndCompare(
        "10\n"
        "08:00 22:00\n"
        "100\n"
        "07:00 2 kasumi 10\n"
        "07:45 1 liara\n"
        "08:15 1 jeff\n"
        "08:55 4 kelly\n"
        "09:38 2 samantha 5\n"
        "10:10 2 miranda 8\n"
        "10:48 4 wrex\n"
        "11:23 3 garrus\n"
        "12:00 1 garrus\n"
        "12:31 4 edi\n"
        "13:05 1 samantha\n"
        "13:47 2 edi 4\n"
        "14:24 3 liara\n"
        "14:58 4 jeff\n"
        "15:43 3 miranda\n"
        "16:21 3 kasumi\n"
        "16:52 2 thane 4\n"
        "17:26 4 kelly\n"
        "18:12 2 edi 6\n"
        "18:54 1 liara\n"
        "19:38 1 kelly\n"
        "20:11 1 miranda\n"
        "20:49 1 ashley\n"
        "21:23 4 garrus\n"
        "22:07 4 miranda\n",
        map
    );

    map = {
        {1, {0, TimeStamp()}},
        {2, {0, TimeStamp()}},
        {3, {500, TimeStamp(4, 14)}},
        {4, {200, TimeStamp(1, 18)}},
        {5, {500, TimeStamp(4, 25)}},
    };

    LoadAndCompare(
        "5\n"
        "08:00 22:00\n"
        "100\n"
        "07:00 1 thane\n"
        "08:19 1 wrex\n"
        "09:34 3 wrex\n"
        "10:30 1 kaidan\n"
        "11:25 1 garrus\n"
        "12:21 4 kaidan\n"
        "13:21 2 garrus 3\n"
        "14:09 2 garrus 3\n"
        "15:01 2 wrex 4\n"
        "16:19 4 wrex\n"
        "17:35 2 garrus 5\n"
        "18:41 3 garrus\n"
        "20:00 1 ashley\n"
        "21:14 2 garrus 5\n"
        "22:13 1 shepard\n",
        map
    );
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
