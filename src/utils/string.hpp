#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <concepts>


template <typename StringLike, std::output_iterator<std::string> OutIt>
void Split(const StringLike &src, OutIt dest) {
    std::istringstream ss(src);
    std::string token;
    while (ss >> token) {
        *dest++ = std::move(token);
        token = std::string{};
    }
}

template <typename StringLike, std::output_iterator<std::string> OutIt>
void Split(const StringLike &src, int maxsplit, OutIt dest) {
    std::istringstream ss(src);
    std::string token;
    for (int i = 0; i < maxsplit; ++i) {
        if (!(ss >> token)) {
           return;
        }
        *dest++ = std::move(token);
        token = std::string{};
    }
    if (!ss.eof()) {
        std::copy(std::istreambuf_iterator<char>(ss),
                  std::istreambuf_iterator<char>(),
                  std::back_inserter(token));

        *dest = std::move(token);
    }
}

template <typename StringLike>
bool IsDigit(const StringLike &src) {
    return std::size(src) != 0 && std::all_of(std::begin(src), std::end(src), [](char c) { return std::isdigit(c); });
}

template <std::size_t N>
bool IsDigit(const char (&src)[N]) {
    return N >= 2 && std::all_of(src, src + N - 1, [](char c) { return std::isdigit(c); });
}

bool IsDigit(const char *src);