#pragma once

#include <iostream>


template <typename T>
    requires requires(const T &t) {
        Dump(std::declval<std::ostream&>(), t);
    }
std::ostream &operator <<(std::ostream &os, const T& t) {
    Dump(os, t);
    return os;
}

template <typename T>
    requires requires(T &t) {
        Load(std::declval<std::istream&>(), t);
    }
std::istream &operator >>(std::istream &is, T& t) {
    Load(is, t);
    return is;
}