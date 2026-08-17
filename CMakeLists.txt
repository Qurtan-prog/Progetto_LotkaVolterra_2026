cmake_minimum_required(VERSION 3.16)
project(LotkaVolterra LANGUAGES CXX)

# Upgrade to C++20 for defaulted comparison operators (operator== = default)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)
find_package(Threads REQUIRED)

add_executable(lotka_volterra
    main.cpp
    simulation.cpp
    Plotter.cpp
    input.cpp
)

target_include_directories(lotka_volterra PRIVATE include)
target_link_libraries(lotka_volterra PRIVATE sfml-graphics sfml-window sfml-system Threads::Threads)
