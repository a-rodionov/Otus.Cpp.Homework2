#include "ip.h"
#include <algorithm>

IP::IP(const std::vector<std::string>& ip_str)
{
  if(4 != ip_str.size())
    throw std::invalid_argument("String represantation of IP contains wrong number of elements.");

  std::transform(std::cbegin(ip_str),
                std::cend(ip_str),
                ip_parts.begin(),
                [] (const auto& ip_string)
                {
                  auto ip_part = std::stoul(ip_string);
                  if(255 < ip_part)
                    throw std::invalid_argument("String represantation of ip element is greater than 255.");
                  return  ip_part;
                });
}

bool operator>(const IP& lhs, const IP& rhs)
{
  return lhs.ip_parts > rhs.ip_parts;
}

std::ostream& operator<<(std::ostream& out, const IP& ip)
{
  for (auto ip_part = std::cbegin(ip.ip_parts); ip_part != std::cend(ip.ip_parts); ++ip_part)
  {
    if (ip_part != std::cbegin(ip.ip_parts))
    {
      out << ".";
    }
    out << static_cast<unsigned int>(*ip_part);
  }
  out << std::endl;
  return out;
}