cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets Test)

set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE SERVER_SOURCES
    ../../src/server/*.h
    ../../src/server/*.cpp
)

add_executable(
  server_test
  ${SERVER_SOURCES}
  TestServer.cpp

)
target_link_libraries(
  server_test
  GTest::gtest GTest::gtest_main
  Qt6::Core Qt6::Network Qt6::Widgets Qt6::Test
)

add_test(NAME ServerTest COMMAND server_test)
