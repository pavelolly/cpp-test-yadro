#include "timestamp.hpp"

#include <ctime>
#include <iostream>

// TODO: require leading zeros

void Dump(std::ostream &os, const TimeStamp &src) {
    os << src.GetHours() << ":" << src.GetMinutes();
}

std::istream &Load(std::istream &is, TimeStamp &dest) {
    is >> std::chrono::parse("%H:%M", dest);
    return is;
}