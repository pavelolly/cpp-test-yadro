#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <concepts>

template <std::output_iterator<std::string> OutIt>
void Split(const std::string &src, OutIt dest) {
    std::istringstream ss(src);
    std::string token;
    while (ss >> token) {
        *dest++ = std::move(token);
        token = std::string{};
    }
}

template <std::output_iterator<std::string> OutIt>
void Split(const std::string &src, OutIt dest, int maxsplit) {
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
        std::copy(std::istreambuf_iterator<char>(ss), std::istreambuf_iterator<char>(), std::back_inserter(token));
        *dest = std::move(token);
    }
}

bool IsDigit(const std::string src);