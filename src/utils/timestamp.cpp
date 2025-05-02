#include "timestamp.hpp"

void Dump(std::ostream &os, const TimeStamp &time) {
    os << time.GetHours() << ":" << time.GetMinutes();
}