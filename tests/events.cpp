#include <gtest/gtest.h>
#include <iostream>

#include "event.hpp"

TEST(Events, Creation) {
    TimeStamp time(12, 34);

    using enum EventId;

    auto DumpAndCompare = [](const Event &event, std::string_view str) {
        std::ostringstream ss;
        Dump(ss, event);
        ASSERT_EQ(ss.view(), str);
    };

    auto ev = Event::Create<IN_CLIENT_CAME>(time, ClientInfo("client1"));
    DumpAndCompare(ev, "12:34 1 client1");

    ev = Event::Create<IN_CLIENT_START>(time, ClientTable("client2", 12));
    DumpAndCompare(ev, "12:34 2 client2 12");

    ev = Event::Create<IN_CLIENT_WAIT>(time, ClientInfo("client3"));
    DumpAndCompare(ev, "12:34 3 client3");

    ev = Event::Create<IN_CLIENT_GONE>(time, ClientInfo("client3"));
    DumpAndCompare(ev, "12:34 4 client3");

    ev = Event::Create<OUT_CLIENT_GONE>(time, ClientInfo("client1"));
    DumpAndCompare(ev, "12:34 11 client1");

    ev = Event::Create<OUT_CLIENT_START>(time, ClientTable("client2", 14));
    DumpAndCompare(ev, "12:34 12 client2 14");

    ev = Event::Create<OUT_ERROR>(time, Error("message"));
    DumpAndCompare(ev, "12:34 13 message");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}