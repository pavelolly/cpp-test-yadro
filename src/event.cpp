#include "event.hpp"

void Dump(std::ostream &os, const Event &event) {
    Dump(os, event.GetTime());
    os << " " << static_cast<int>(event.GetId()) << " ";
    event.GetBodySerializer().Dump(os);
}