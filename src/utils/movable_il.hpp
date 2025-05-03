#pragma once

#include <initializer_list>
#include <iterator>

/*
    stolen from:
    https://stackoverflow.com/questions/46737054/vectorunique-ptra-using-initialization-list?utm_source=chatgpt.com
*/

// namespace {

template<typename T>
struct movable_il {
    mutable T t;
    operator T() const&& { return std::move(t); }
    movable_il(T&& in): t(std::move(in)) {}
};

// } // namespace

template<class Container, typename T = typename Container::value_type>
Container FromIl(std::initializer_list<movable_il<T>> il) {
  return Container(std::make_move_iterator(il.begin()), std::make_move_iterator(il.end()));
}