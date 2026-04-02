cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets Test)

set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE CLIENT_SOURCES
    ../../src/client/*.h
    ../../src/client/*.cpp
)

add_executable(
  client_test
  ${CLIENT_SOURCES}
  TestClient.cpp
)
target_link_libraries(
  client_test
  GTest::gtest GTest::gtest_main
  Qt6::Core Qt6::Network Qt6::Widgets Qt6::Test
)

add_test(NAME ClientTest COMMAND client_test)
