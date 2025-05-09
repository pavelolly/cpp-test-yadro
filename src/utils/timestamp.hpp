#pragma once

#include <chrono>

struct TimeStamp : public std::chrono::minutes {
    TimeStamp(int hour = 0, int minutes = 0)
        : std::chrono::minutes(hour * 60 + minutes)
    {}

    TimeStamp(std::chrono::minutes other)
        : std::chrono::minutes(other)
    {}
    
    static TimeStamp Invalid() {
        return TimeStamp(24, 0);
    }

    int GetHours() const {
        return (*this / 60).count();
    }
    int GetMinutes() const {
        return (*this % 60).count();
    }
};

void Dump(std::ostream &os, const TimeStamp &time);

// guaranteed to consume 5 chars from stream
std::istream &Load(std::istream &is, TimeStamp &time);
