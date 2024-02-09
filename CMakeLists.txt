cmake_minimum_required(VERSION 3.15)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

#
# Project details
#
project(
  "bpftime-new-api-poc"
  VERSION 0.1.0
  LANGUAGES C CXX
)

add_library("bpftime-new-api-poc" SHARED ./nginx_module_attach_impl.cpp)
