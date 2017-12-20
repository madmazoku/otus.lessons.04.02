cmake_minimum_required(VERSION 3.2)

project(ip_filter VERSION 0.4.$ENV{TRAVIS_BUILD_NUMBER})

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost COMPONENTS unit_test_framework program_options REQUIRED)

configure_file(in.version.h version.h)

add_executable(ip_filter main.cpp)

add_executable(ip_filter_test test.cpp)

set_target_properties(ip_filter ip_filter_test PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

set_target_properties(ip_filter_test PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_STATIC_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(ip_filter
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
)

target_link_libraries(ip_filter_test
    ${Boost_LIBRARIES}
)

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT madmazoku@gmail.com)

include(CPack)

enable_testing()

add_test(ip_filter ip_filter_test)