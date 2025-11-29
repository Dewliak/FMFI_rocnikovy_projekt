//
// Created by dewliak on 10/29/25.
//

#ifndef ROCNIKOVY_PROJEKT_EDGE_H
#define ROCNIKOVY_PROJEKT_EDGE_H

#include <set>
#include <set>

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

namespace std {
    template<>
    struct hash<Edge> {
        size_t operator()(const Edge& e) const noexcept {
            int v1 = std::min(e.first, e.second);
            int v2 = std::max(e.first, e.second);

            return std::hash<long long>()(
                (static_cast<long long>(v1) << 32) ^ static_cast<unsigned long long>(v2)
            );
        }
    };
}



#endif //ROCNIKOVY_PROJEKT_EDGE_H