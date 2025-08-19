cmake_minimum_required(VERSION 3.21)
project(Quommoty)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

find_package(QuantLib CONFIG REQUIRED 1.39)

add_executable(Quommoty
    "src/main.cpp"
    "include/utils/print_utils.h"
    "include/utils/io_utils.h"
    "src/utils/io_utils.cpp"
    "include/utils/date_utils.h"
    "src/Pricer.cpp"
    "include/Pricer.h"
    "src/ElectricityFuturesPricer.cpp"
    "include/ElectricityFuturesPricer.h"
    "src/CrudeFuturesPricer.cpp"
    "include/CrudeFuturesPricer.h"
    "tests/demo.cpp"
    "tests/demo.h")

target_include_directories(Quommoty PRIVATE include tests)

target_link_libraries(Quommoty PRIVATE QuantLib::QuantLib)

if (MSVC)
  target_compile_options(Quommoty PRIVATE /W4 /permissive-)
else()
  target_compile_options(Quommoty PRIVATE -Wall -Wextra -Wpedantic)
endif()