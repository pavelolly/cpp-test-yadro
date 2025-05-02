#include "string.hpp"

#include <algorithm>

bool IsDigit(const std::string src) {
    return !src.empty() && std::all_of(src.begin(), src.end(), [](char c) { return std::isdigit(c); });
}