cmake_minimum_required(VERSION 4.0)
project(rocnikovy_projekt)

set(CMAKE_CXX_STANDARD 20)

# --- CaDiCaL ---
include_directories(${CADICAL_DIR}/src)
link_directories(${CAIDCAL_DIR}/build)

# --- Main executable ---
add_executable(rocnikovy_projekt
        src/main.cpp
        src/Graph.cpp
        include/Graph.h
        src/ColoringSAT.cpp
        include/ColoringSAT.h
        src/Matrix.cpp
        include/Matrix.h
        src/CadicalSAT.cpp
        include/CadicalSAT.h
)
target_link_libraries(rocnikovy_projekt PRIVATE cadical)

# --- GoogleTest ---
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# --- Tests ---
add_executable(MyTests
        tests/test_main.cpp
        tests/test_graph_coloring.cpp
        src/Matrix.cpp
        src/Graph.cpp
        src/ColoringSAT.cpp
        include/SATInterface.h
        src/SATInterface.cpp
        include/CadicalSAT.h
        src/CadicalSAT.cpp
        include/EdgeList.h
        src/EdgeList.cpp
        include/PMSAT.h
        src/PMSAT.cpp
        include/IGraph.h
        src/IGraph.h
        src/AdjacencyListGraph.cpp
        include/AdjacencyListGraph.h
        tests/test_igraph.cpp
        include/Edge.h
)

# Important: link both gtest and cadical
target_link_libraries(MyTests
        PRIVATE
        GTest::gtest_main
        cadical
)

# Ensure the include path for CaDiCaL is visible
target_include_directories(MyTests PRIVATE
        ${CADICAL_DIR}/src
        ${CMAKE_SOURCE_DIR}/include
)

include(GoogleTest)
gtest_discover_tests(MyTests)
