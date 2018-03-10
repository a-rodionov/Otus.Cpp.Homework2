#include "version.h"
#include "ip_filter.h"

#include <vector>
#include <algorithm>

#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_suite_main)

BOOST_AUTO_TEST_CASE(test_version_valid)
{
  BOOST_CHECK( version() > version_info() );
}

BOOST_AUTO_TEST_CASE(test_ip_filter_valid)
{
  std::vector<std::vector<int>> ip_pool{{1,46,84,93}, {46,94,114,245}, {97,51,213,87}};

  auto any_ip_filter = make_ip_filter<int>();
  BOOST_CHECK( ip_pool.size() == std::count_if(std::cbegin(ip_pool), std::cend(ip_pool), any_ip_filter) );

  auto first_byte_46_filter = make_ip_filter(make_ip_part_comparator(std::equal_to<int>(), 46));
  for(const auto& ip : ip_pool)
  {
    if(first_byte_46_filter(ip))
      BOOST_CHECK( ip == ip_pool.at(1) );
  }

  reinit_ip_filter(
    any_ip_filter,
    make_ip_part_comparator(any_value<int>(), 0),
    make_ip_part_comparator(any_value<int>(), 0),
    make_ip_part_comparator(std::equal_to<int>(), 213));
  for(const auto& ip : ip_pool)
  {
    if(any_ip_filter(ip))
      BOOST_CHECK( ip == ip_pool.at(2) );
  }

  auto second_byte_46_filter = make_ip_filter(
    make_ip_part_comparator(any_value<int>(), 0),
    make_ip_part_comparator(std::equal_to<int>(), 46));
  auto third_byte_46_filter = make_ip_filter(
    make_ip_part_comparator(any_value<int>(), 0),
    make_ip_part_comparator(any_value<int>(), 0),
    make_ip_part_comparator(std::equal_to<int>(), 46));
  auto fourth_byte_46_filter = make_ip_filter(
    make_ip_part_comparator(any_value<int>(), 0),
    make_ip_part_comparator(any_value<int>(), 0),
    make_ip_part_comparator(any_value<int>(), 0),
    make_ip_part_comparator(std::equal_to<int>(), 46));
  BOOST_CHECK( 2 == std::count_if(std::cbegin(ip_pool), std::cend(ip_pool), [&](const auto& ip) {
    return (first_byte_46_filter(ip) || second_byte_46_filter(ip) || third_byte_46_filter(ip) || fourth_byte_46_filter(ip));
  }) );
  
}

BOOST_AUTO_TEST_SUITE_END()
