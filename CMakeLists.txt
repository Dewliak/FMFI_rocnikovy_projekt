message(STATUS "HIGHS_DIR = '${HIGHS_DIR}'")
message(STATUS "CADICAL_DIR = '${CADICAL_DIR}'")

cmake_minimum_required(VERSION 4.0)
project(rocnikovy_projekt)

set(CMAKE_CXX_STANDARD 20)

# --- logger

include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.13.0
)

FetchContent_MakeAvailable(spdlog)



# --- include my libraries ---
include_directories(${CMAKE_SOURCE_DIR}/include)

# --- CaDiCaL ---
include_directories(${CADICAL_DIR}/src)
link_directories(${CADICAL_DIR}/build)

# --- HIGHS
add_library(highs SHARED IMPORTED
        include/ilp/DefectILP.h
        include/Pipeline.h
        src/PipeLine.cpp)

set_target_properties(highs PROPERTIES
        IMPORTED_LOCATION "${HIGHS_DIR}/build/lib64/libhighs.so"
        INTERFACE_INCLUDE_DIRECTORIES "${HIGHS_DIR}/highs;${HIGHS_DIR}/build"
)


add_library(graph_lib
        src/graph/AdjacencyListGraph.cpp
        src/graph/EdgeList.cpp
        src/graph/Graph.cpp
        src/graph/GraphAlgorithms.cpp
        include/defect_search/structures.h
        include/defect_search/utilities.h
        src/defect_search/utilities.cpp
        src/defect_search/defectComputing.cpp
        include/defect_search/defectComputing.h
        include/defect_search/assignmentGeneration.h
        src/defect_search/assignmentGeneration.cpp
        include/export/exportPython.h
        src/export/exportPython.cpp
        include/logger.h
        src/logger.cpp

)

# ---------- SAT library ----------
add_library(sat_lib
        src/sat/CadicalSAT.cpp
        src/sat/ColoringSAT.cpp
        src/sat/ISAT.cpp
        src/sat/DefectSAT.cpp
        include/sat/DefectSAT.h
        include/ilp/DefectILP.h
        src/ilp/DefectILP.cpp
        include/Pipeline.h
        src/PipeLine.cpp
        include/logger.h
        src/logger.cpp
)

target_link_libraries(sat_lib PUBLIC highs)
target_link_libraries(sat_lib PRIVATE cadical)
target_link_libraries(graph_lib PUBLIC highs)


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
        include/ilp/DefectILP.h
        include/Pipeline.h
        src/PipeLine.cpp
        include/logger.h
        src/logger.cpp
)

target_link_libraries(rocnikovy_projekt
        PRIVATE
        graph_lib
        sat_lib
        pm_lib
        matrix_lib
        cadical
        spdlog::spdlog
)

target_link_libraries(graph_lib PUBLIC spdlog::spdlog)
target_link_libraries(sat_lib PUBLIC spdlog::spdlog)
target_link_libraries(pm_lib PUBLIC spdlog::spdlog)

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
        tests/test_high_lpsolver.cpp
        src/PipeLine.cpp
        tests/test_compare_method_bf_ilp.cpp
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