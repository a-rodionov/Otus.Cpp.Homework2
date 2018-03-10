cmake_minimum_required(VERSION 3.2)

# Определение PATCH_VERSION для локальной сборки и сборки на travis
set(PATCH_VERSION 1)
if($ENV{TRAVIS_BUILD_NUMBER})
  set(PATCH_VERSION $ENV{TRAVIS_BUILD_NUMBER})
endif()

project(homework2 VERSION 1.0.${PATCH_VERSION})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

configure_file(version_numbers.h.in version_numbers.h)

# Создание целей
add_executable(ip_filter main.cpp)

add_library(ip_filter_lib STATIC version.cpp ip_filter.h)

add_executable(ip_filter_test_main test_main.cpp)

# Настройка целей

# для всех целей
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wpedantic -Wall -Wextra)

target_link_libraries(ip_filter ip_filter_lib)

target_compile_definitions(ip_filter_test_main PRIVATE BOOST_TEST_DYN_LINK)
target_link_libraries(ip_filter_test_main 
  Boost::unit_test_framework
  ip_filter_lib
)

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT camelot58@gmail.com)

include (CPack)

enable_testing()

add_test(test_version_valid ip_filter_test_main)
add_test(test_ip_filter_valid ip_filter_test_main)
