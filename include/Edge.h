//
// Created by dewliak on 10/29/25.
//

#ifndef ROCNIKOVY_PROJEKT_EDGE_H
#define ROCNIKOVY_PROJEKT_EDGE_H

struct Edge {
    int first;
    int second;

    Edge( int va, int vb) : first(va), second(vb) {};

    static Edge create( int va,  int vb) {
        return Edge(va, vb);
    }

    int getFirst() const{return first;}
    int getSecond() const{return second;}

    std::set<int> getVertices() const {
        return std::set<int>({first, second});
    }

    bool operator==(const Edge& other) const {
        return (first == other.first && second == other.second) || (first == other.second && second == other.first);
    }

    bool operator<(const Edge& other) const {
        if (first != other.first) return first < other.first;
        return second < other.second;
    }
};


#endif //ROCNIKOVY_PROJEKT_EDGE_H