cmake_minimum_required(VERSION 3.29)
project(giga-chsrv)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fsanitize=address")

add_executable(server_app server/server.cpp)
