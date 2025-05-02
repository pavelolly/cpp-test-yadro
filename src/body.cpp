#include "body.hpp"

ClientInfo::ClientInfo(std::string client_name)
    : client_name(std::move(client_name))
{}

ClientTable::ClientTable(std::string client_name, int table_number)
    : client_name(std::move(client_name)), table_number(table_number)
{}

Error::Error(std::string_view message) : message(message) {}


void Dump(std::ostream &os, const ClientInfo &client_info) {
    os << client_info.client_name;
}

void Dump(std::ostream &os, const ClientTable &client_table) {
    os << client_table.client_name << " " << client_table.table_number;
}

void Dump(std::ostream &os, const Error &error) {
    os << error.message;
}