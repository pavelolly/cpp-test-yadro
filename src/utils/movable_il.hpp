#pragma once

#include <initializer_list>
#include <iterator>

/*
    stolen from:
    https://stackoverflow.com/questions/46737054/vectorunique-ptra-using-initialization-list
*/

namespace {

template<typename T>
struct MovableIlItem {
    mutable T t;
    operator T() const&& { return std::move(t); }
    MovableIlItem(T&& t): t(std::move(t)) {}
};

} // namespace

template <typename Container>
Container FromIl(std::initializer_list<MovableIlItem<typename Container::value_type>> il) {
  return Container(std::make_move_iterator(il.begin()), std::make_move_iterator(il.end()));
}