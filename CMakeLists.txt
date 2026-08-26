cmake_minimum_required(VERSION 3.17)  # Ninja Multi-Config needs 3.17+
project(LotkaVolterra LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# --- Main executable ---
add_executable(lotka_volterra
    main.cpp
    simulation.cpp
    plot_axes.cpp
    plotter.cpp
    input.cpp
)

target_include_directories(lotka_volterra PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lotka_volterra PRIVATE sfml-graphics sfml-window sfml-system)

# --- Test executable ---
enable_testing()

add_executable(simulation_test
    simulation_test.cpp
    simulation.cpp
    input.cpp
)

target_include_directories(simulation_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME simulation_test COMMAND simulation_test)