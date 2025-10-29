cmake_minimum_required(VERSION 4.0)
project(rocnikovy_projekt)

set(CMAKE_CXX_STANDARD 20)

# --- CaDiCaL ---
include_directories(${CADICAL_DIR}/src)
link_directories(${CAIDCAL_DIR}/build)

# --- Main executable ---
add_executable(rocnikovy_projekt
        src/main.cpp
        src/graph/Graph.cpp
        include/graph/Graph.h
        src/sat/ColoringSAT.cpp
        include/sat/ColoringSAT.h
        src/Matrix.cpp
        include/Matrix.h
        src/sat/CadicalSAT.cpp
        include/sat/CadicalSAT.h
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
        src/graph/Graph.cpp
        src/sat/ColoringSAT.cpp
        include/sat/ISAT.h
        src/sat/ISAT.cpp
        include/sat/CadicalSAT.h
        src/sat/CadicalSAT.cpp
        include/graph/EdgeList.h
        src/graph/EdgeList.cpp
        include/perfect_matching/PMSAT.h
        src/perfect_matching/PMSAT.cpp
        include/graph/IGraph.h
        src/graph/IGraph.h
        src/graph/AdjacencyListGraph.cpp
        include/graph/AdjacencyListGraph.h
        tests/test_igraph.cpp
        include/graph/Edge.h
        include/graph/GraphAlgorithms.h
        src/graph/GraphAlgorithms.cpp
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
