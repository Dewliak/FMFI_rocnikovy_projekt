//
// Created by dewliak on 4/2/26.
//

#include <gtest/gtest.h>
#include "Pipeline.h"
#include "defect_search/utilities.h"
TEST(MethodTesting, Simple_BF_ILP1) {

    std::string s = "I?h]@eOWG";
    GraphColoringData data = generateColoring(s, 0, 4);


    Solution ilpSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::ILP);
    Solution bruteforceSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::BruteForce);

    int ilpDitance = hammingDistanceForDefect(data.originalSolution,ilpSolution);
    int bfDitance = hammingDistanceForDefect(data.originalSolution,bruteforceSolution);

    EXPECT_EQ(ilpDitance, bfDitance);
}


TEST(MethodTesting, Simple_BF_ILP2) {

    std::string s = "Q?gY@eOGGC?B_??@g_??DO?O?GW";
    GraphColoringData data = generateColoring(s, 0, 8);


    Solution ilpSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::ILP);
    Solution bruteforceSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::BruteForce);

    int ilpDitance = hammingDistanceForDefect(data.originalSolution,ilpSolution);
    int bfDitance = hammingDistanceForDefect(data.originalSolution,bruteforceSolution);

    EXPECT_EQ(ilpDitance, bfDitance);
}

TEST(MethodTesting, Simple_BF_ILP3) {

    std::string s = "Q?gY@eOGGC?B_??@g_??DO?O?GW";
    GraphColoringData data = generateColoring(s, 12, 15);


    Solution ilpSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::ILP);
    Solution bruteforceSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::BruteForce);

    int ilpDitance = hammingDistanceForDefect(data.originalSolution,ilpSolution);
    int bfDitance = hammingDistanceForDefect(data.originalSolution,bruteforceSolution);

    EXPECT_EQ(ilpDitance, bfDitance);
}

TEST(MethodTesting, Simple_BF_ILP4) {

    std::string s = "Q?gY@eOGGC?B_??@g_??DO?O?GW";
    GraphColoringData data = generateColoring(s, 6,1);


    Solution ilpSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::ILP);
    Solution bruteforceSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::BruteForce);

    int ilpDitance = hammingDistanceForDefect(data.originalSolution,ilpSolution);
    int bfDitance = hammingDistanceForDefect(data.originalSolution,bruteforceSolution);

    EXPECT_EQ(ilpDitance, bfDitance);
}

TEST(MethodTesting, Simple_BF_ILP5) {

    std::string s = "Q?gY@eOGGC?B_??@g_??DO?O?GW";
    GraphColoringData data = generateColoring(s, 15, 17);


    Solution ilpSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::ILP);
    Solution bruteforceSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::BruteForce);

    int ilpDitance = hammingDistanceForDefect(data.originalSolution,ilpSolution);
    int bfDitance = hammingDistanceForDefect(data.originalSolution,bruteforceSolution);

    EXPECT_EQ(ilpDitance, bfDitance);
}


TEST(MethodTesting, ALLPAIRS_BF_ILP1) {


    std::string s = "I?h]@eOWG";

    AdjacencyListGraph graph(s);

    vector<int> vertices = graph.getVertices();

    for (int i = 0; i < vertices.size(); i++) {
        for (int j = i+1; j < vertices.size(); j++) {
            int v1 = vertices.at(i);
            int v2 = vertices.at(j);
            if (!graph.containsEdge(Edge(v1,v2))) {
                continue;
            }
            cout << "Vertex pair: " << v1 << " " << v2 << endl;
            GraphColoringData data = generateColoring(s, v1,v2);



            Solution ilpSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::ILP);
            Solution bruteforceSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::BruteForce);

            int ilpDitance = hammingDistanceForDefect(data.originalSolution,ilpSolution);
            int bfDitance = hammingDistanceForDefect(data.originalSolution,bruteforceSolution);

            EXPECT_EQ(ilpDitance, bfDitance);
        }
    }
}

TEST(MethodTesting, ALLPAIRS_BF_ILP2) {

    int countDifferent = 0;
    int all = 0;
    std::string s = "Q?hY@eOGG??B_??@g???T?a??@g";

    AdjacencyListGraph graph(s);

    vector<int> vertices = graph.getVertices();

    for (int i = 0; i < vertices.size(); i++) {
        for (int j = i+1; j < vertices.size(); j++) {
            int v1 = vertices.at(i);
            int v2 = vertices.at(j);
            if (!graph.containsEdge(Edge(v1,v2))) {
                continue;
            }
            cout << "Vertex pair: " << v1 << " " << v2 << endl;
            GraphColoringData data = generateColoring(s, v1,v2);



            Solution ilpSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::ILP);
            Solution bruteforceSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::BruteForce);

            int ilpDitance = hammingDistanceForDefect(data.originalSolution,ilpSolution);
            int bfDitance = hammingDistanceForDefect(data.originalSolution,bruteforceSolution);

            all++;
            if (ilpDitance != bfDitance) {
                countDifferent++;
                cout << "DIFFERENCE AT: " << v1 << ", " << v2 << endl;
            }
            EXPECT_EQ(ilpDitance, bfDitance);
        }
    }

    cout << "Different amount: " << countDifferent <<  "/" << all << endl;
}



TEST(MethodTesting, ALLPAIRS_ALLCOLORS_BF_ILP2) {

    long long countDifferent = 0;
    long long  all = 0;
    std::string s = "I?h]@eOWG";

    AdjacencyListGraph graph(s);

    vector<int> vertices = graph.getVertices();

    for (int i = 0; i < vertices.size(); i++) {
        for (int j = i+1; j < vertices.size(); j++) {
            int v1 = vertices.at(i);
            int v2 = vertices.at(j);
            if (!graph.containsEdge(Edge(v1,v2))) {
                continue;
            }
            cout << "Vertex pair: " << v1 << " " << v2 << endl;
            std::vector<GraphColoringData> allData = generateAllColoring(s, v1, v2);


            for (GraphColoringData data: allData) {
                Solution ilpSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::ILP);
                Solution bruteforceSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::BruteForce);

                int ilpDitance = hammingDistanceForDefect(data.originalSolution,ilpSolution);
                int bfDitance = hammingDistanceForDefect(data.originalSolution,bruteforceSolution);

                if (ilpDitance != bfDitance) {
                    countDifferent++;
                }
                all++;

                EXPECT_EQ(ilpDitance, bfDitance);

            }
        }
    }

    cout << "Different amount: " << countDifferent <<  "/" << all << endl;
}

TEST(MethodTesting, BIG_BF_ILP1) {

    std::string s = "c?GY?EO?GC??P??BP???_GO???O?E???A?G?OC??a?????F?@?????_C???O?@G???AO?????G??I?????AK??@@?????`G?G_??????E@";
    GraphColoringData data = generateColoring(s, 0, 20);


    Solution ilpSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::ILP);
    //Solution bruteforceSolution = findClosestWithDefectThree(data.originalGraphFormat,data.modifiedGraphEdgeList,data.originalSolution,data.baseline,SearchStrategy::BruteForce);

    int ilpDitance = hammingDistanceForDefect(data.originalSolution,ilpSolution);
    //int bfDitance = hammingDistanceForDefect(data.originalSolution,bruteforceSolution);

    //EXPECT_EQ(ilpDitance, bfDitance);
}