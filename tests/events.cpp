#include <gtest/gtest.h>
#include <iostream>

#include "event.hpp"

TEST(Events, Creation) {
    auto DumpAndCompare = [](const Event &event, std::string_view str) {
        std::ostringstream ss;
        ss << event;
        EXPECT_EQ(ss.view(), str);
    };

    using enum EventId;
    
    TimeStamp time(12, 34);

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

    std::cout << "Here to check if standart operator << still works " << 69420 << "\n";
}

// TODO: TEST(Events, EventIds)

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}