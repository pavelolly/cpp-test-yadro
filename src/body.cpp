#include "body.hpp"

void Dump(std::ostream &os, const ClientInfo &src) {
    os << src.client_name;
}

std::istream &Load(std::istream &is, ClientInfo &dest) {
    ClientInfo client_info;
    is >> client_info.client_name;
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
    is >> client_table.client_name >> client_table.table_number;
    if (!is) {
        return is;
    }

    dest = std::move(client_table);
    return is;
}

void Dump(std::ostream &os, const Error &src) {
    os << src.message;
}