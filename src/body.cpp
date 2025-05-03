#include "body.hpp"

#include <algorithm>

void Dump(std::ostream &os, const ClientInfo &src) {
    os << src.client_name;
}

// TODO: handle client_name to be [a-zA-Z0-9]+

namespace {

std::istream &LoadClientName(std::istream &is, std::string &dest) {
    std::string name;
    is >> name;
    if (!is) {
        return is;
    }
    if (std::any_of(name.begin(), name.end(), [](char c) { return !std::isdigit(c) && !std::isalpha(c) && c != '_' && c != '-'; })) {
        is.setstate(std::ios::failbit);
        return is;
    }

    dest = std::move(name);
    return is;
}

} // namespace


std::istream &Load(std::istream &is, ClientInfo &dest) {
    ClientInfo client_info;
    LoadClientName(is, client_info.client_name);
    if (!is) {
        return is;
    }

    dest = std::move(client_info);
    return is;
}

void Dump(std::ostream &os, const ClientTable &src) {
    os << src.client_name << " " << src.table_number;
}

std::istream &Load(std::istream &is, ClientTable &dest) {
    ClientTable client_table;
    LoadClientName(is, client_table.client_name);
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