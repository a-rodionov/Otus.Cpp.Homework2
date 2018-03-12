#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <stdint.h>

using IP = std::vector<uint8_t>;

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

std::ostream& operator<<(std::ostream& out, const IP& ip)
{
  for (auto ip_part = std::cbegin(ip); ip_part != std::cend(ip); ++ip_part)
  {
    if (ip_part != std::cbegin(ip))
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
    std::vector<IP> ip_pool;

    // Функцию split  по указанию оставил без имзменений.
    // А вот хранимый тип данных изменил.
    for (std::string line; std::getline(std::cin, line);)
    {
      IP ip;
      ip.reserve(4);

      std::vector<std::string> v = split(line, '\t');
      v = split(v.at(0), '.');     

      for(const auto& ip_string : v)
        ip.push_back(std::stoul(ip_string));
      ip_pool.emplace_back(std::move(ip));
    }

    std::sort(std::begin(ip_pool), std::end(ip_pool), std::greater<decltype(*std::begin(ip_pool))>());
    for(const auto& ip : ip_pool)
      std::cout << ip;

    for(const auto& ip : ip_pool)
    {
      if (1 == ip.at(0))
        std::cout << ip;
    }

    for(const auto& ip : ip_pool)
    {
      if ((46 == ip.at(0)) && (70 == ip.at(1)))
        std::cout << ip;
    }

    for(const auto& ip : ip_pool)
    {
      if (std::any_of(std::cbegin(ip), std::cend(ip), [](const auto& ip_part) { return 46 == ip_part; }))
        std::cout << ip;
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}