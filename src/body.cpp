#include "body.hpp"

#include <algorithm>
#include <regex>

#include "errors.hpp"

void Dump(std::ostream &os, const body::ClientInfo &src) {
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


std::istream &Load(std::istream &is, body::ClientInfo &dest) {
    body::ClientInfo client_info;
    LoadClientName(is, client_info.name);
    if (!is) {
        return is;
    }

    dest = std::move(client_info);
    return is;
}

void Dump(std::ostream &os, const body::ClientTable &src) {
    os << src.name << " " << src.table_number;
}

std::istream &Load(std::istream &is, body::ClientTable &dest) {
    body::ClientTable client_table;
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

void Dump(std::ostream &os, const body::Error &src) {
    os << src.message;
}

std::istream &Load(std::istream &is, body::Error &dest) {
    std::string str;
    is >> str;
    if (!is) {
        return is;
    }

    std::string_view message = error::GetError(str);
    if (message.empty()) {
        is.setstate(std::ios::failbit);
        return is;
    }

    dest.message = message;
    return is;
}
