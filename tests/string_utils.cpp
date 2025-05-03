#include <gtest/gtest.h>
#include <iostream>
#include <iterator>

#include "utils/string.hpp"

TEST(StringUtils, IsDigit) {
    EXPECT_TRUE(IsDigit("123"));
    EXPECT_TRUE(IsDigit("08475"));
    EXPECT_FALSE(IsDigit(""));
    EXPECT_FALSE(IsDigit("-123"));
    EXPECT_FALSE(IsDigit("0x18"));
    EXPECT_FALSE(IsDigit("0.125"));

    EXPECT_TRUE(IsDigit((const char *)"123"));
    EXPECT_TRUE(IsDigit((const char *)"08475"));
    EXPECT_FALSE(IsDigit((const char *)""));
    EXPECT_FALSE(IsDigit((const char *)"-123"));
    EXPECT_FALSE(IsDigit((const char *)"0x18"));
    EXPECT_FALSE(IsDigit((const char *)"0.125"));

    using namespace std::literals::string_literals;
    using namespace std::literals::string_view_literals;

    EXPECT_TRUE(IsDigit("123"s));
    EXPECT_TRUE(IsDigit("08475"s));
    EXPECT_FALSE(IsDigit(""s));
    EXPECT_FALSE(IsDigit("-123"s));
    EXPECT_FALSE(IsDigit("0x18"s));
    EXPECT_FALSE(IsDigit("0.125"s));

    EXPECT_TRUE(IsDigit("123"sv));
    EXPECT_TRUE(IsDigit("08475"sv));
    EXPECT_FALSE(IsDigit(""sv));
    EXPECT_FALSE(IsDigit("-123"sv));
    EXPECT_FALSE(IsDigit("0x18"sv));
    EXPECT_FALSE(IsDigit("0.125"sv));
}

TEST(StringUtils, Split) {
    std::vector<std::string> splitted;
    splitted.reserve(10);

    auto SplitAndCompare = [&](std::string str, std::vector<std::string> expected) {
        splitted.clear();
        Split(str, std::back_inserter(splitted));

        EXPECT_EQ(splitted, expected);
    };

    SplitAndCompare("", {});
    SplitAndCompare(" \n\t", {});

    SplitAndCompare("Hello, World!",        {"Hello,", "World!"});
    SplitAndCompare("  Hello,   World!   ", {"Hello,", "World!"});
    SplitAndCompare("  H ello ,   Wor ld!   ", {"H", "ello", ",", "Wor", "ld!"});
    SplitAndCompare("\n H\t\rello , \v Wor ld! \n\n ", {"H", "ello", ",", "Wor", "ld!"});
}

TEST(StringUtils, SplitMaxSplit) {
    std::vector<std::string> splitted;
    splitted.reserve(10);

    auto SplitAndCompare = [&](std::string str, int maxsplit, std::vector<std::string> expected) {
        splitted.clear();
        Split(str, maxsplit, std::back_inserter(splitted));

        EXPECT_EQ(splitted, expected);
    };

    SplitAndCompare("", 1, {});
    SplitAndCompare(" \n\t", 12, {});

    SplitAndCompare("Hello, World!", 1,        {"Hello,", " World!"});
    SplitAndCompare("  Hello,   World!   ", 2, {"Hello,", "World!", "   "});
    SplitAndCompare("  H ello ,   Wor ld!   ", 3, {"H", "ello", ",", "   Wor ld!   "});
    SplitAndCompare("\n H\t\rello , \v Wor ld! \n\n ", 10, {"H", "ello", ",", "Wor", "ld!"});

    SplitAndCompare("Hello, World!", -1, {"Hello, World!"});
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}