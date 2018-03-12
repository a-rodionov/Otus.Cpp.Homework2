#include "version.h"

#include <vector>
#include <algorithm>
#include <numeric>

#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_suite_main)

BOOST_AUTO_TEST_CASE(test_version_valid)
{
  BOOST_CHECK( version() > version_info() );
}

using IP = std::vector<uint8_t>;

BOOST_AUTO_TEST_CASE(test_ip_filter_valid)
{
  std::vector<IP> ip_pool{{1,46,84,93}, {46,94,114,245}, {97,51,213,87}};

  for(const auto& ip : ip_pool)
  {
    if(46 == ip.at(0))
      BOOST_CHECK( ip == ip_pool.at(1) );
  }

  for(const auto& ip : ip_pool)
  {
    if(213 == ip.at(2))
      BOOST_CHECK( ip == ip_pool.at(2) );
  }

  BOOST_CHECK( 2 == std::count_if(std::cbegin(ip_pool),
                                  std::cend(ip_pool),
                                  [](const auto& ip)
                                  {
                                    return std::any_of(std::cbegin(ip),
                                                      std::cend(ip),
                                                      [](const auto& ip_part) { return 46 == ip_part; } );
                                  }));

  BOOST_CHECK( 224 == std::accumulate(std::cbegin(ip_pool.at(0)), std::cend(ip_pool.at(0)), 0) );
  
}

BOOST_AUTO_TEST_SUITE_END()
