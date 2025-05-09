#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <utility>
#include <concepts>

template <typename String, typename OutIt>
void Split(const String &src, OutIt dest) {
    std::istringstream ss(src);
    std::string token;
    while (ss >> token) {
        *dest++ = std::exchange(token, std::string{});
    }
}

template <typename String, typename OutIt>
void Split(const String &src, OutIt dest, int maxsplit) {
    std::istringstream ss(src);
    std::string token;
    for (int i = 0; i < maxsplit; ++i) {
        if (!(ss >> token)) {
           return;
        }
        *dest++ = std::exchange(token, std::string{});
    }
    
    if (!ss.eof()) {
        std::copy(std::istreambuf_iterator<char>(ss),
                  std::istreambuf_iterator<char>(),
                  std::back_inserter(token));

        *dest = std::move(token);
    }
}

template <typename String, typename OutIt>
void Split(const String &src, OutIt dest, char delim) {
    std::istringstream ss(src);
    std::string token; 
    while (std::getline(ss, token, delim)) {
        *dest++ = std::exchange(token, std::string{});
    }
}

template <typename String>
bool IsDigit(const String &src) {
    return std::size(src) != 0 && std::all_of(std::begin(src), std::end(src), [](char c) { return std::isdigit(c); });
}

bool IsDigit(const char *src);

template <std::size_t N>
bool IsDigit(const char (&src)[N]) {
    return N >= 1 && std::all_of(src, src + N, [](char c) { return std::isdigit(c); });
}