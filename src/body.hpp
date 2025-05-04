#pragma once

#include <iostream>
#include <string>
#include <string_view>

namespace body {

struct ClientInfo {
    std::string name;

    bool operator ==(const ClientInfo &) const = default;
};

struct ClientTable {
    std::string name;
    int table_number;

    bool operator ==(const ClientTable &) const = default;
};

struct Error {
    std::string_view message;

    bool operator ==(const Error &) const = default;
};

} // namespace body

void Dump(std::ostream &os, const body::ClientInfo &srcload);
std::istream &Load(std::istream &is, body::ClientInfo &dest);

void Dump(std::ostream &os, const body::ClientTable &src);
std::istream &Load(std::istream &is, body::ClientTable &dest);

void Dump(std::ostream &os, const body::Error &src);
// no need to load error