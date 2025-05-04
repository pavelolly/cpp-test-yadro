#pragma once

#include <iostream>
#include <string>
#include <string_view>

struct Body {
    virtual bool equal(const Body &other) const = 0;
    
    bool operator ==(const Body &other) const {
        return equal(other);
    }
    bool operator !=(const Body &other) const {
        return !operator ==(other);
    }

    virtual ~Body() = default;
};

// TODO: use compiler generated operator ==()... somehow...
struct ClientInfo : Body {
    std::string client_name;

    ClientInfo() = default;
    ClientInfo(std::string client_name)
        : client_name(std::move(client_name))
    {}

    bool equal(const Body &other) const override {
        if (auto *p = dynamic_cast<const ClientInfo *>(&other)) {
            return client_name == p->client_name;
        }
        return false;
    }
};

struct ClientTable : Body {
    std::string client_name;
    int table_number;

    ClientTable() = default;
    ClientTable(std::string client_name, int table_number)
        : client_name(std::move(client_name)), table_number(table_number)
    {}

    bool equal(const Body &other) const override {
        if (auto *p = dynamic_cast<const ClientTable *>(&other)) {
            return client_name == p->client_name
                   && table_number == p->table_number;
        }
        return false;
    }
};

struct Error : Body {
    std::string_view message;

    Error() = default;
    Error(std::string_view message)
        : message(std::move(message))
    {}

    bool equal(const Body &other) const override {
        if (auto *p = dynamic_cast<const Error *>(&other)) {
            return message == p->message;
        }
        return false;
    }
};

void Dump(std::ostream &os, const ClientInfo &srcload);
std::istream &Load(std::istream &is, ClientInfo &dest);

void Dump(std::ostream &os, const ClientTable &src);
std::istream &Load(std::istream &is, ClientTable &dest);

void Dump(std::ostream &os, const Error &src);
// no need to load error
