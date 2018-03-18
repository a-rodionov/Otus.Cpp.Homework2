#pragma once

#include <string>
#include <vector>
#include <array>
#include <stdint.h>
#include <iostream>

struct IP
{
  explicit IP(const std::vector<std::string>& ip_str);
  std::array<uint8_t, 4> ip_parts;
};

bool operator>(const IP& lhs, const IP& rhs);

std::ostream& operator<<(std::ostream& out, const IP& ip);

struct ignore{};

template<typename T>
constexpr bool operator==(const ignore& lhs, const T& rhs)
{
  return true;
}

template<typename T>
constexpr bool operator==(const T& lhs, const ignore& rhs)
{
  return true;
}
