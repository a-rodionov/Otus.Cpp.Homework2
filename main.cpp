#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <stdint.h>
#include "ip_filter.h"

std::vector<std::string> split(const std::string &str, char d)
{
  std::vector<std::string> r;

  std::string::size_type start = 0;
  std::string::size_type stop = str.find_first_of(d);
  while (stop != std::string::npos)
  {
    r.push_back(str.substr(start, stop - start));

    start = stop + 1;
    stop = str.find_first_of(d, start);
  }

  r.push_back(str.substr(start));

  return r;
}

template<typename Container>
void print_ip(const Container& ip)
{
  for (auto ip_part = ip.cbegin(); ip_part != ip.cend(); ++ip_part)
  {
    if (ip_part != ip.cbegin())
    {
      std::cout << ".";
    }
    std::cout << static_cast<unsigned int>(*ip_part);
  }
  std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
  try
  {
    std::vector<std::vector<uint8_t> > ip_pool;

    // Функцию split  по указанию оставил без имзменений.
    // А вот хранимый тип данных изменил.
    for (std::string line; std::getline(std::cin, line);)
    {
      std::vector<std::string> v = split(line, '\t');
      v = split(v.at(0), '.');
      std::vector<uint8_t> ip_parts;
      std::transform(v.begin(), v.end(), std::back_inserter(ip_parts), [](const auto& str) { return std::stoul(str); });
      ip_pool.push_back(ip_parts);
    }

    std::sort(ip_pool.begin(), ip_pool.end(), std::greater<decltype(*ip_pool.begin())>());

    std::for_each(ip_pool.cbegin(), ip_pool.cend(), print_ip<decltype(*ip_pool.cbegin())>);

    // Самый простой способ, но нет повторяемости кроме как Ctrl+C, Ctrl+V.
    // Из-за этого возникают вопросы с тестированием данного решения.
    //std::for_each(
    //  ip_pool.cbegin(),
    //  ip_pool.cend(),
    //  [](const auto& ip) {
    //  if (1 == ip.at(0))
    //    print_ip(ip); });

    //std::for_each(
    //  ip_pool.cbegin(),
    //  ip_pool.cend(),
    //  [](const auto& ip) {
    //  if ((46 == ip.at(0)) && (70 == ip.at(1)))
    //    print_ip(ip); });

    //std::for_each(
    //  ip_pool.cbegin(),
    //  ip_pool.cend(),
    //  [](const auto& ip) {
    //  if (std::any_of(ip.cbegin(), ip.cend(), [](auto ip_part) { return 46 == ip_part; }))
    //    print_ip(ip); });

    // Более сложный способ с применением экземрляров класса IpFilter.
    auto ip_filter_1 = make_ip_filter(make_ip_part_comparator(std::equal_to<uint8_t>(), 1));
    for(const auto& ip : ip_pool)
    {
      if(ip_filter_1(ip))
        print_ip(ip);
    }

    reinit_ip_filter(
      ip_filter_1,
      make_ip_part_comparator(std::equal_to<uint8_t>(), 46),
      make_ip_part_comparator(std::equal_to<uint8_t>(), 70));
    for (const auto& ip : ip_pool)
    {
      if (ip_filter_1(ip))
        print_ip(ip);
    }

    reinit_ip_filter(
      ip_filter_1,
      make_ip_part_comparator(std::equal_to<uint8_t>(), 46));
    auto ip_filter_2 = make_ip_filter(
      make_ip_part_comparator(any_value<uint8_t>(), 0),
      make_ip_part_comparator(std::equal_to<uint8_t>(), 46));
    auto ip_filter_3 = make_ip_filter(
      make_ip_part_comparator(any_value<uint8_t>(), 0),
      make_ip_part_comparator(any_value<uint8_t>(), 0),
      make_ip_part_comparator(std::equal_to<uint8_t>(), 46));
    auto ip_filter_4 = make_ip_filter(
      make_ip_part_comparator(any_value<uint8_t>(), 0),
      make_ip_part_comparator(any_value<uint8_t>(), 0),
      make_ip_part_comparator(any_value<uint8_t>(), 0),
      make_ip_part_comparator(std::equal_to<uint8_t>(), 46));
    for (const auto& ip : ip_pool)
    {
      if (ip_filter_1(ip) || ip_filter_2(ip) || ip_filter_3(ip) || ip_filter_4(ip))
        print_ip(ip);
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}