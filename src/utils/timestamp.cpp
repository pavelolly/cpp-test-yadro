#include "timestamp.hpp"

#include <ctime>
#include <iostream>
#include <regex>

void Dump(std::ostream &os, const TimeStamp &src) {
    std::ios fmt_state(nullptr);
    fmt_state.copyfmt(os); // preserve formatting settings

    os << std::setw(2) << std::setfill('0') << src.GetHours()
       << ":"
       << std::setw(2) << src.GetMinutes();
    
    os.copyfmt(fmt_state);
}

std::istream &Load(std::istream &is, TimeStamp &dest) {
    static std::regex pattern(R"(\d\d:\d\d)");

    std::istream::sentry s(is);
    if (!s) {
        return is;
    }

    std::string time(5, '\0');
    is.read(&*time.begin(), 5);

    if (!is) {
        return is;
    }

    if (!std::regex_match(time, pattern)) {
        is.setstate(std::ios::failbit);
        return is;
    }

    int hours   = std::stoi(time.substr(0, 2));
    int minutes = std::stoi(time.substr(3));
    if (hours >= 24 || minutes >= 60) {
        is.setstate(std::ios::failbit);
        return is;
    };

    dest = TimeStamp(hours, minutes);
    return is;
}