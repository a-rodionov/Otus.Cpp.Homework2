#pragma once

#include <string>
#include <vector>
#include <array>
#include <stdint.h>
#include <iostream>

constexpr auto variadic_and()
{
  return true;
}

template<typename Head, typename ... Tail>
constexpr auto variadic_and(const Head& head, const Tail& ... tail)
{
  return head && variadic_and(tail...);
}

template<typename Array, size_t ... idx, typename ... Args>
constexpr auto filter_is_equal_impl(const Array& ip, std::index_sequence<idx...>, Args ... args)
{
  return variadic_and((ip[idx] == args)...);
}

template<typename T, std::size_t N, typename ... Args, typename Indices = std::make_index_sequence<sizeof...(Args)>>
constexpr auto filter_is_equal(const std::array<T,N>& ip, Args ... args)
{
  static_assert(N >= sizeof...(args), "There're more items for comparing than array size!");
  return filter_is_equal_impl(ip, Indices{}, args...);
}

void process(std::ostream& out, std::istream& in);
