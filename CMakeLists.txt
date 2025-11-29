cmake_minimum_required(VERSION 4.0)
project(rocnikovy_projekt)

set(CMAKE_CXX_STANDARD 20)

# --- include my libraries ---
include_directories(${CMAKE_SOURCE_DIR}/include)

# --- CaDiCaL ---
include_directories(${CADICAL_DIR}/src)
link_directories(${CADICAL_DIR}/build)
add_library(graph_lib
        src/graph/AdjacencyListGraph.cpp
        src/graph/EdgeList.cpp
        src/graph/Graph.cpp
        src/graph/GraphAlgorithms.cpp
)

# ---------- SAT library ----------
add_library(sat_lib
        src/sat/CadicalSAT.cpp
        src/sat/ColoringSAT.cpp
        src/sat/ISAT.cpp
)

target_link_libraries(sat_lib PRIVATE cadical)

# ---------- Perfect matching library ----------
add_library(pm_lib
        src/perfect_matching/PMSAT.cpp
)

# ---------- Matrix ----------
add_library(matrix_lib
        src/Matrix.cpp
)

# ---------- Main executable ----------
add_executable(rocnikovy_projekt
        src/main.cpp
)

target_link_libraries(rocnikovy_projekt
        PRIVATE
        graph_lib
        sat_lib
        pm_lib
        matrix_lib
        cadical
)

# ------------ GoogleTest ------------
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(MyTests
        tests/test_graph_coloring.cpp
        tests/test_igraph.cpp
)

target_link_libraries(MyTests
        PRIVATE
        graph_lib
        sat_lib
        pm_lib
        matrix_lib
        GTest::gtest_main
        cadical
)

include(GoogleTest)
gtest_discover_tests(MyTests)