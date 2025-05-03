#pragma once

#include <iostream>
#include <string>
#include <string_view>

struct Body {
    virtual ~Body() = default;
};

struct ClientInfo : Body {
    std::string client_name;

    ClientInfo() = default;
    ClientInfo(std::string client_name)
        : client_name(std::move(client_name))
    {}
};

struct ClientTable : Body {
    std::string client_name;
    int table_number;

    ClientTable() = default;
    ClientTable(std::string client_name, int table_number)
        : client_name(std::move(client_name)), table_number(table_number)
    {}
};

struct Error : Body {
    std::string_view message;

    Error() = default;
    Error(std::string_view message)
        : message(std::move(message))
    {}
};

void Dump(std::ostream &os, const ClientInfo &src);
std::istream &Load(std::istream &is, ClientInfo &dest);

void Dump(std::ostream &os, const ClientTable &src);
std::istream &Load(std::istream &is, ClientTable &dest);

void Dump(std::ostream &os, const Error &src);
// no need to load error
