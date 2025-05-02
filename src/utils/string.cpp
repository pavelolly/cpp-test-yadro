#include <cstring>
#include <cctype>
#include <algorithm>

bool IsDigit(const char *src) {
    auto length = std::strlen(src);
    return length != 0 && std::all_of(src, src + length, [](char c) { return std::isdigit(c); });
}