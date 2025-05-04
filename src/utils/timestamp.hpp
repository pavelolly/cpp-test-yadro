#pragma once

#include <chrono>

struct TimeStamp : public std::chrono::minutes {
    TimeStamp() = default;
    TimeStamp(int hour, int minutes)
        : std::chrono::minutes(hour * 60 + minutes)
    {}
    
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