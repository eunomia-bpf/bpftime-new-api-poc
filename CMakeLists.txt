cmake_minimum_required(VERSION 3.15)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


#
# Project details
#
project(
  "bpftime-new-api-poc"
  VERSION 0.1.0
  LANGUAGES C CXX
)

add_subdirectory(./bpftime)

add_library(nginx_module_attach_impl SHARED ./minimal_syscall_server.cpp ./nginx_module_attach_impl.cpp)

add_dependencies(nginx_module_attach_impl bpftime-new-api-poc)
target_link_libraries(nginx_module_attach_impl PRIVATE bpftime-new-api-poc)
target_include_directories(nginx_module_attach_impl PRIVATE ./bpftime)
