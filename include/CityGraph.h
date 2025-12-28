#ifndef CITYGRAPH_H
#define CITYGRAPH_H

#include <vector>
#include <string>
#include <list>

// Structure to represent a road (Edge)
struct Edge {
    int destination;
    int weight; // Time in minutes
};

class CityGraph {
private:
    int numVertices;
    std::vector<std::list<Edge>> adjLists;

public:
    // Constructor
    CityGraph(int vertices);

    // Core Functions
    bool loadGraph(const std::string& filename);
    void addEdge(int src, int dest, int weight);
    void removeEdge(int src, int dest);
    
    // The "Brain" (Dijkstra - Time Calculation)
    int dijkstra(int startVertex, int endVertex);

    // The "Diagnostic Tool" (BFS - Connectivity Check)
    std::vector<int> getUnreachableNodes(int startNode);
};

#endif