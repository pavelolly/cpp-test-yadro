#include "body.hpp"

void Dump(std::ostream &os, const ClientInfo &client_info) {
    os << client_info.client_name;
}

void Dump(std::ostream &os, const ClientTable &client_table) {
    os << client_table.client_name << " " << client_table.table_number;
}

void Dump(std::ostream &os, const Error &error) {
    os << error.message;
}