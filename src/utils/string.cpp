#include <cstring>
#include <cctype>
#include <algorithm>

#include "string.hpp"

bool IsDigit(const char *src) {
    auto length = std::strlen(src);
    return length != 0 && std::all_of(src, src + length, [](char c) { return std::isdigit(c); });
}