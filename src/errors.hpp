#pragma once

#include <string_view>

namespace error {

inline constexpr std::string_view YOU_SHALL_NOT_PASS = "YouShallNotPass";
inline constexpr std::string_view NOT_OPEN_YET       = "NotOpenYet";
inline constexpr std::string_view PLACE_IS_BUSY      = "PlaceIsBusy";
inline constexpr std::string_view CLIENT_UNKNOWN     = "ClientUnknown";
inline constexpr std::string_view CAN_WAIT_NO_LONGER = "ICanWaitNoLonger!";

} // namespace error