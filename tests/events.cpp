#include <gtest/gtest.h>
#include <iostream>

#include "events.hpp"

TEST(Events, Creation) {
    Event::TimeStamp time(12*60 + 34); // 12:34

    using enum Event::Id;

    auto DumpAndCompare = [](const Event &event, std::string_view str) {
        std::ostringstream ss;
        event.Dump(ss);
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