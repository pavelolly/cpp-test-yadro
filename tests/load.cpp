#include <gtest/gtest.h>
#include <iostream>

#include "utils/timestamp.cpp"
#include "utils/stream_operators.hpp"

TEST(Load, TimeStamp) {

#define LoadAndCompare(src, time) \
    do { \
        std::istringstream ss((src)); \
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
        std::istringstream ss((src)); \
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

TEST(Load, EventBody) {

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