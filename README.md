**Názov:** Kritické snarky a pokrývanie perfektnými páreniami

**Cieľom projektu** je preskúmať vzťahy medzi kritickými snarkami, indexom perfektného párenia a defektom. Najmä pôjde o skúmanie hypotéz, že každý kritický snark má PMI = 4 a defekt 3.
To bude založené na analýze známych snarkov pomocou počítačového programu. Pre daný kritický snark G a dvojicu jeho vrcholov u, v vieme nájsť zafarbenie grafu G – {u, v}. Program sa pokúsi 
nájsť také *3 perfektné párovania grafu G nechávajúce 3 nepokryté grafy, ktoré sa od nájdeného zafarbenia grafu G – {u, v} líšia čo najmenej*. Ideálne sa podarí vyvinúť program, ktorý (aspoň v niektorých prípadoch) 
takéto párovania nájde na základe zisteného farbenia. 

# Definitions:

    - Configuarion = Given a u,v pair of vertices, that has edge between them in graph G
    - Color Configuration = u,v configuarion with a coloring C

# Install:
    1. Install CADICAL:
    2. In Cmake options add -DCADICAL_DIR=path/to/cadical
    3. Install HiGHS
    4. Install compiler flag -DHIGHS_DIR=path/to/highs

# Usage:
    The binary itself is callable with these parameters:
    -g(neccesary) : graph in graph6 string format
    -f(neccesary) : filename to export to, it will be opened with append (default preset: "../export_data/export.csv" - might be changed later)
    
    Togglable ( if they are exclusive, the last flag from the two are counted)
    - Coloring:
        --one_color - find the answer for one random coloring, for a given given configuration
        --all_colors(default) - finds answer for all the colorings for a given configuration

    - Pair
        --one_pair - find answer for one random configuration (almost always with the smallest i,j edge) 
        --all_pairs(default) - find answer for all configuration

## Pipeline
    
    1. Python runner reads graph6 format from the file
    2. The runner starts the binary with graph 
    2.1 Finds a u,v pair where (u,v) in E(G)
    2.2 Create G' = G - {u,v}
    2.3 Find a coloring for G' with SAT(Cadical) - baseline
    2.4 Find a solution with ILP(HiGHS), perfect matching with defect 3, with minimum Hamming distance from the original coloring
```cpp
    int hammingDistanceForDefect(Solution original, Solution defect) {
    int distance = 0;
    
        set<Edge> originalEdges = set<Edge>();
    
        for (const Edge &e: original.M1) originalEdges.insert(e);
        for (const Edge &e: original.M2) originalEdges.insert(e);
        for (const Edge &e: original.M3) originalEdges.insert(e);
    
        for (const Edge &e: originalEdges) {
            if (original.M1.contains(e) != defect.M1.contains(e)) {
                distance++;
            }
    
            if (original.M2.contains(e) != defect.M2.contains(e)) {
                distance++;
            }
    
            if (original.M3.contains(e) != defect.M3.contains(e)) {
                distance++;
            }
        }
    
        return distance;
    }

```
    2.5 Save the solution in the export file in a csv format, the columns:
        1. graph6 string format
        2. strategy: ILP, Bruteforce
        3. Minimum hamming distance
        4. u vertex
        5. v vertex
        6. Baseline coloring - edges are in a canonical order, and every edge has a number from {0,1,2,3}. 
            - 0 means no color, 1-3 means which color. e.g. 000123132003221231331122321331312
        7.,8.,9. which perfect matching binary format:
            - 0 not in matching i
            - 1 in matching i
            - e.g.: 100100100010001001001101000000110
        

# Done:
    - Finding answers for all the critical snarks with vertices n <= 32 (started on n=34), 1 coloring/configuraion,
        so with the flags --one_color --all_pairs. Exported them into export_data/bulk
    - Python runner for running the graph calculations

    - TESTING - write it
    - Testing:
        Have a bruteforce solution, which calculates all possible 3 perfect matchings with defect 3 and checks with the 
        minimum distance.
        - For the critical snark with vertex count 10 I did the following tests:
            - Simple 1 coloring and 1 configuration
            - all coloring/all configuration
        - Checked a few larger graphs, but for them the bruteforce approach quickly becomes too slow to have any use. But 
            for the case n = 10 did check every possible configuration and compared it to the ILP solution
# Todo:
    - Finding 1 coloring/configuration for all graphs with vertices n <= 36 (est. time 10 days of computation)
    - Finding all coloring/configuration for all graph with vertices n <= 36
    - Some kind of dashboard for analysing the properties, for example the deviation for minimum hamming distance
  
# Notes:
    - For a given configuration it calculates maxiammly between 1-2 mins (usually few seconds) for large n(>=34), 