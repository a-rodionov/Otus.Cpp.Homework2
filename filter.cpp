#include "filter.h"
#include "ip.h"
#include "utils.h"
#include <algorithm>
#include <iterator>
#include <set>

void process(std::ostream& out, std::istream& in)
{
  std::multiset<IP, std::greater<IP>> ip_pool;

  for (std::string line; std::getline(in, line);)
  {
    std::vector<std::string> v = split(line, '\t');
    ip_pool.emplace(split(v.at(0), '.'));
  }

  std::copy(std::cbegin(ip_pool),
            std::cend(ip_pool),
            std::ostream_iterator<decltype(ip_pool)::value_type>{out});

  std::copy_if(std::cbegin(ip_pool),
              std::cend(ip_pool),
              std::ostream_iterator<decltype(ip_pool)::value_type>{out},
              [] (const IP& ip) { return filter_is_equal(ip.ip_parts, 1); });

  std::copy_if(std::cbegin(ip_pool),
              std::cend(ip_pool),
              std::ostream_iterator<decltype(ip_pool)::value_type>{out},
              [] (const IP& ip) { return filter_is_equal(ip.ip_parts, 46, 70); });

  std::copy_if(std::cbegin(ip_pool),
              std::cend(ip_pool),
              std::ostream_iterator<decltype(ip_pool)::value_type>{out},
              [] (const IP& ip) { return (std::any_of(std::cbegin(ip.ip_parts),
                                                      std::cend(ip.ip_parts),
                                                      [](const auto& ip_part) { return 46 == ip_part; })); });

  // It can be just so easy
/*    
  for(const auto& ip : ip_pool)
    out << ip;

  for(const auto& ip : ip_pool)
  {
    if (1 == ip.at(0))
      out << ip;
  }

  for(const auto& ip : ip_pool)
  {
    if ((46 == ip.at(0)) && (70 == ip.at(1)))
      out << ip;
  }

  for(const auto& ip : ip_pool)
  {
    if (std::any_of(std::cbegin(ip), std::cend(ip), [](const auto& ip_part) { return 46 == ip_part; }))
      out << ip;
  }
*/
}