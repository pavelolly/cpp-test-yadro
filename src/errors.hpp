#pragma once

#include <string_view>

namespace error {

inline constexpr std::string_view YOU_SHALL_NOT_PASS = "YouShallNotPass";
inline constexpr std::string_view NOT_OPEN_YET       = "NotOpenYet";
inline constexpr std::string_view PLACE_IS_BUSY      = "PlaceIsBusy";
inline constexpr std::string_view CLIENT_UNKNOWN     = "ClientUnknown";
inline constexpr std::string_view CAN_WAIT_NO_LONGER = "ICanWaitNoLonger!";

inline std::string_view GetError(std::string_view str) {
    if (str == YOU_SHALL_NOT_PASS) {
        return YOU_SHALL_NOT_PASS;
    }
    if (str == NOT_OPEN_YET) {
        return NOT_OPEN_YET;
    }
    if (str == PLACE_IS_BUSY) {
        return PLACE_IS_BUSY;
    }
    if (str == CLIENT_UNKNOWN) {
        return CLIENT_UNKNOWN;
    }
    if (str == CAN_WAIT_NO_LONGER) {
        return CAN_WAIT_NO_LONGER;
    }
    return "";
}

} // namespace error