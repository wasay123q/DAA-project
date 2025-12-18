#ifndef CITYGRAPH_H
#define CITYGRAPH_H

#include <vector>
#include <list>
#include <utility>
#include <string>
#include <limits>

// Represents an edge: {destination_node, weight}
typedef std::pair<int, int> Edge;
const int INF = std::numeric_limits<int>::max();

class CityGraph {
private:
    int numNodes;
    // Adjacency list: adj[u] contains list of {v, weight}
    std::vector<std::vector<Edge>> adj;

public:
    // Constructor
    CityGraph(int nodes);

    // Core Graph Operations
    void addRoad(int u, int v, int weight);
    void removeRoad(int u, int v);
    
    // Algorithmic Features
    // CLO3: Greedy Strategy (Dijkstra) [cite: 3]
    int getShortestPath(int start, int end, std::vector<int>& path);
    
    // Connectivity Check (BFS) [cite: 28]
    std::vector<int> getUnreachableNodes(int startNode);

    // Helpers
    void printGraph() const;
};

#endif