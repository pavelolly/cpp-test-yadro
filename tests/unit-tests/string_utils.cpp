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

#define I(...) { __VA_ARGS__ }

TEST(StringUtils, Split) {
    std::vector<std::string> splitted;
    splitted.reserve(10);
    
#define SplitAndCompare(str, expected) \
    do {                               \
        splitted.clear();              \
        Split((str), std::back_inserter(splitted)); \
        EXPECT_EQ(splitted, std::vector<std::string>(expected)); \
    } while(0)

    SplitAndCompare("", {});
    SplitAndCompare(" \n\t", {});

    SplitAndCompare("Hello, World!",           I("Hello,", "World!"));
    SplitAndCompare("  Hello,   World!   ",    I("Hello,", "World!"));
    SplitAndCompare("  H ello ,   Wor ld!   ", I("H", "ello", ",", "Wor", "ld!"));
    SplitAndCompare("\n H\t\rello , \v Wor ld! \n\n ", I("H", "ello", ",", "Wor", "ld!"));

#undef SplitAndCompare
}

TEST(StringUtils, SplitMaxSplit) {
    std::vector<std::string> splitted;
    splitted.reserve(10);

#define SplitAndCompare(str, maxsplit, expected) \
    do {                  \
        splitted.clear(); \
        Split((str), (maxsplit), std::back_inserter(splitted));  \
        EXPECT_EQ(splitted, std::vector<std::string>(expected)); \
    } while(0)

    SplitAndCompare("", 1, {});
    SplitAndCompare(" \n\t", 12, {});

    SplitAndCompare("Hello, World!",                   1,  I("Hello,", " World!"));
    SplitAndCompare("  Hello,   World!   ",            2,  I("Hello,", "World!", "   "));
    SplitAndCompare("  H ello ,   Wor ld!   ",         3,  I("H", "ello", ",", "   Wor ld!   "));
    SplitAndCompare("\n H\t\rello , \v Wor ld! \n\n ", 10, I("H", "ello", ",", "Wor", "ld!"));

    SplitAndCompare("Hello, World!", -1, I("Hello, World!"));

#undef SplitAndCompare
}

TEST(StringUtils, SplitDelim) {
    std::vector<std::string> splitted;
    splitted.reserve(10);

#define SplitAndCompare(str, delim, expected) \
    do {                  \
        splitted.clear(); \
        Split((str), (delim), std::back_inserter(splitted));  \
        EXPECT_EQ(splitted, std::vector<std::string>(expected)); \
    } while(0)

    SplitAndCompare("", 'a', {});
    SplitAndCompare(" \n\t", '\n', I(" ", "\t"));

    SplitAndCompare("Hello, World!",                   ' ',   I("Hello,", "World!"));
    SplitAndCompare("  Hello,   World!   ",            ' ',   I("", "", "Hello,", "", "", "World!", "", ""));
    SplitAndCompare("  H\nello , \n Wor ld! \n\n",     '\n',  I("  H", "ello , ", " Wor ld! ", ""));
    SplitAndCompare("\n H\t\rello , \v Wor ld! \n\n ", 'o',   I("\n H\t\rell", " , \v W", "r ld! \n\n "));

    SplitAndCompare("Hello, World!", '!', I("Hello, World"));

#undef SplitAndCompare
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}