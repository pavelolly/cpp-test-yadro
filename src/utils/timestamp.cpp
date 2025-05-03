#include "timestamp.hpp"

#include <ctime>
#include <iostream>

// TODO: require leading zeros

void Dump(std::ostream &os, const TimeStamp &src) {
    os << src.GetHours() << ":" << src.GetMinutes();
}

std::istream &Load(std::istream &is, TimeStamp &dest) {
    std::chrono::hours hours;
    is >> std::chrono::parse("%H", hours);
    if (!is) {
        is.setstate(std::ios::failbit);
        return is;
    }
    if (int h = hours.count(); h < 0 || h > 23) {
        is.setstate(std::ios::failbit);
        return is;
    }

    if (is.peek() != ':') {
        is.setstate(std::ios::failbit);
        return is;
    }
    is.ignore();

    std::chrono::minutes minutes;
    is >> std::chrono::parse("%M", minutes);
    if (!is) {
        is.setstate(std::ios::failbit);
        return is;
    }
    if (int m = minutes.count(); m < 0 || m > 59) {
        is.setstate(std::ios::failbit);
        return is;
    }

    dest = TimeStamp(hours.count(), minutes.count());
    return is;
}