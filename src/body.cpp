#include "body.hpp"

#include <algorithm>
#include <regex>

void Dump(std::ostream &os, const ClientInfo &src) {
    os << src.name;
}

namespace {

std::istream &LoadClientName(std::istream &is, std::string &dest) {
    static std::regex pattern(R"([a-z0-9_-]+)");

    std::string name;
    is >> name;
    if (!is) {
        return is;
    }

    if (!std::regex_match(name, pattern)) {
        is.setstate(std::ios::failbit);
        return is;
    }

    dest = std::move(name);
    return is;
}

} // namespace


std::istream &Load(std::istream &is, ClientInfo &dest) {
    ClientInfo client_info;
    LoadClientName(is, client_info.name);
    if (!is) {
        return is;
    }

    dest = std::move(client_info);
    return is;
}

void Dump(std::ostream &os, const ClientTable &src) {
    os << src.name << " " << src.table_number;
}

std::istream &Load(std::istream &is, ClientTable &dest) {
    ClientTable client_table;
    LoadClientName(is, client_table.name);
    if (!is) {
        return is;
    }

    is >> client_table.table_number;
    if (!is) {
        return is;
    }

    dest = std::move(client_table);
    return is;
}

void Dump(std::ostream &os, const Error &src) {
    os << src.message;
}