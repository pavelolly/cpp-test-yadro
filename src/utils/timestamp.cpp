#include "timestamp.hpp"

#include <ctime>
#include <iostream>

void Dump(std::ostream &os, const TimeStamp &src) {
    std::ios fmt_state(nullptr);
    fmt_state.copyfmt(os); // preserve formatting settings

    os << std::setw(2) << std::setfill('0') << src.GetHours()
       << ":"
       << std::setw(2) << src.GetMinutes();
    
    os.copyfmt(fmt_state);
}

std::istream &Load(std::istream &is, TimeStamp &dest) {
    std::istream::sentry s(is);
    if (!s) {
        return is;
    }

    // read hours
    int h1 = is.peek();
    if (h1 < '0' || h1 > '2') {
        is.setstate(std::ios::failbit);
        return is;
    }
    is.ignore();
    int h2 = is.peek();
    if (!std::isdigit(h2)) {
        is.setstate(std::ios::failbit);
        return is;
    }
    is.ignore();

    int hours = (h1 - '0') * 10 + (h2 - '0');
    if (hours < 0 || hours > 23) {
        is.setstate(std::ios::failbit);
        return is;
    }

    // read ':'
    if (is.peek() != ':') {
        is.setstate(std::ios::failbit);
        return is;
    }
    is.ignore();

    // read minutes
    int m1 = is.peek();
    if (m1 < '0' || m1 > '5') {
        is.setstate(std::ios::failbit);
        return is;
    }
    is.ignore();
    int m2 = is.peek();
    if (!std::isdigit(m2)) {
        is.setstate(std::ios::failbit);
        return is;
    }
    is.ignore();

    int minutes = (m1 - '0') * 10 + (m2 - '0');
    if (minutes < 0 || minutes > 59) {
        is.setstate(std::ios::failbit);
        return is;
    }

    dest = TimeStamp(hours, minutes);
    return is;
}