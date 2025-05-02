#pragma once

#include <chrono>

struct TimeStamp : private std::chrono::minutes {
    TimeStamp(int hour, int minutes)
        : std::chrono::minutes(hour * 60 + minutes)
    {}
    
    int GetHours()   const {
        return (*this / 60).count();
    }
    int GetMinutes() const {
        return (*this % 60).count();
    }
};

void Dump(std::ostream &os, const TimeStamp &time);