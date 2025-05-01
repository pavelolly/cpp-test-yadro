#include <gtest/gtest.h>
#include <iostream>

#include "events.hpp"

TEST(Events, Creation) {
    std::tm tm = {
        .tm_min = 34,
        .tm_hour = 12,
    };

    using enum events::Event::Id;


    auto ev = events::Event::Create<IN_CLIENT_CAME>(tm, events::ClientInfo("client1"));

    auto DumpAndCompare = [&ev](std::string_view str) {
        std::ostringstream ss;
        ev.Dump(ss);
        ASSERT_EQ(ss.view(), str);
    };

    DumpAndCompare("12:34 1 client1");

    ev = events::Event::Create<IN_CLIENT_START>(tm, events::ClientTable("client2", 12));
    DumpAndCompare("12:34 2 client2 12");

    ev = events::Event::Create<IN_CLIENT_WAIT>(tm, events::ClientInfo("client3"));
    DumpAndCompare("12:34 3 client3");

    ev = events::Event::Create<IN_CLIENT_GONE>(tm, events::ClientInfo("client3"));
    DumpAndCompare("12:34 4 client3");

    ev = events::Event::Create<OUT_CLIENT_GONE>(tm, events::ClientInfo("client1"));
    DumpAndCompare("12:34 11 client1");

    ev = events::Event::Create<OUT_CLIENT_START>(tm, events::ClientTable("client2", 14));
    DumpAndCompare("12:34 12 client2 14");

    ev = events::Event::Create<OUT_ERROR>(tm, events::Error("message"));
    DumpAndCompare("12:34 13 message");
}