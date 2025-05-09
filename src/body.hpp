#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "errors.hpp"

namespace body {

struct ClientInfo {
    std::string name;
};

struct ClientTable {
    std::string name;
    int table_number;
};

struct Error {
    std::string_view message;
};

} // namespace body

void Dump(std::ostream &os, const body::ClientInfo &srcload);
std::istream &Load(std::istream &is, body::ClientInfo &dest);

void Dump(std::ostream &os, const body::ClientTable &src);
std::istream &Load(std::istream &is, body::ClientTable &dest);

void Dump(std::ostream &os, const body::Error &src);
std::istream &Load(std::istream &is, body::Error &dest);
