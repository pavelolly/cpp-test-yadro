#pragma once

#include <iostream>
#include <string>
#include <string_view>

struct Body {
    virtual ~Body() = default;
};

struct ClientInfo : Body {
    std::string client_name;

    ClientInfo(std::string client_name)
        : client_name(std::move(client_name))
    {}
};

struct ClientTable : Body {
    std::string client_name;
    int table_number;

    ClientTable(std::string client_name, int table_number)
        : client_name(std::move(client_name)), table_number(table_number)
    {}
};

struct Error : Body {
    std::string_view message;

    Error(std::string_view message)
        : message(message)
    {}
};

void Dump(std::ostream &os, const ClientInfo &client_info);
void Dump(std::ostream &os, const ClientTable &client_table);
void Dump(std::ostream &os, const Error &error);
