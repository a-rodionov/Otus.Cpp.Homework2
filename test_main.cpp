#include "version.h"

#include "ip.h"
#include "filter.h"

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

BOOST_AUTO_TEST_CASE(test_ip_not_present)
{
  std::string testData{"\n"};
  std::istringstream iss(testData);
  std::ostringstream oss;
  BOOST_CHECK_THROW(process(oss, iss), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_ip_element_char_error)
{
  std::string testData{"1.2.b.4\n"};
  std::istringstream iss(testData);
  std::ostringstream oss;
  BOOST_CHECK_THROW(process(oss, iss), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_ip_element_count_less_error)
{
  std::string testData{"1.2.3\n"};
  std::istringstream iss(testData);
  std::ostringstream oss;
  BOOST_CHECK_THROW(process(oss, iss), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_ip_element_count_greater_error)
{
  std::string testData{"1.2.3.4.5\n"};
  std::istringstream iss(testData);
  std::ostringstream oss;
  BOOST_CHECK_THROW(process(oss, iss), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_ip_element_less_error)
{
  std::string testData{"1.2.-1.4\n"};
  std::istringstream iss(testData);
  std::ostringstream oss;
  BOOST_CHECK_THROW(process(oss, iss), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_ip_element_greater_error)
{
  std::string testData{"1.2.1024.4\n"};
  std::istringstream iss(testData);
  std::ostringstream oss;
  BOOST_CHECK_THROW(process(oss, iss), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(test_filter_is_equal)
{
  constexpr std::array<uint8_t, 4> ip {1,2,3,4};

  BOOST_STATIC_ASSERT(true == filter_is_equal(ip, 1, 2, 3, 4));                     // compare all
  BOOST_STATIC_ASSERT(true == filter_is_equal(ip, 1, ignore{}, 3, 4));              // compare all except 2nd
  BOOST_STATIC_ASSERT(true == filter_is_equal(ip, ignore{}, 2, 3));                 // compare 2nd, 3rd and skip 1st
  BOOST_STATIC_ASSERT(true == filter_is_equal(ip, ignore{}, ignore{}, ignore{}, 4));// compare 4th and skip 1st, 2nd, 3rd
  BOOST_STATIC_ASSERT(false == filter_is_equal(ip, 2, 2, 3, 4));                    // not equal
  BOOST_STATIC_ASSERT(false == filter_is_equal(ip, 1, ignore{}, 4, 4));             // not equal
  //BOOST_STATIC_ASSERT(false == filter_is_equal(ip));                                // compile time error if no elements for comparing set
  //BOOST_STATIC_ASSERT(false == filter_is_equal(ip, 2, 2, 3, 4, 5));                 // compile time error if elements to compare > array size
}

BOOST_AUTO_TEST_CASE(test_ip_process_valid)
{
  std::string testData{"1.2.3.4\n"
                      "1.3.3.4\n"
                      "1.3.4.4\n"
                      "1.3.4.5\n"
                      "250.13.87.4\n"
                      "46.70.5.6\n"
                      "70.46.5.6\n"
                      "70.2.46.4\n"};

  std::string result{"250.13.87.4\n"
                    "70.46.5.6\n"
                    "70.2.46.4\n"
                    "46.70.5.6\n"
                    "1.3.4.5\n"
                    "1.3.4.4\n"
                    "1.3.3.4\n"
                    "1.2.3.4\n"
                    "1.3.4.5\n"
                    "1.3.4.4\n"
                    "1.3.3.4\n"
                    "1.2.3.4\n"
                    "46.70.5.6\n"
                    "70.46.5.6\n"
                    "70.2.46.4\n"
                    "46.70.5.6\n"};

  std::istringstream iss(testData);
  std::ostringstream oss;
  process(oss, iss);
  BOOST_CHECK_EQUAL(oss.str(), result);
}

BOOST_AUTO_TEST_SUITE_END()
