#include "../include/CityGraph.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <climits> // For INT_MAX

using namespace std;

// Constructor
CityGraph::CityGraph(int vertices) {
    this->numVertices = vertices;
    adjLists.resize(vertices);
}

// 1. Load Map from File (Robust Version)
bool CityGraph::loadGraph(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "[Error] File not found: " << filename << endl;
        return false;
    }

    int fileNodeCount;
    if (!(file >> fileNodeCount)) {
        cerr << "[Error] Map file is empty or missing node count header." << endl;
        return false;
    }

    // Validation: Check if file matches our allocated memory
    if (fileNodeCount > numVertices) {
        cerr << "[Warning] Map file contains more nodes (" << fileNodeCount 
             << ") than system allocated (" << numVertices << ")." << endl;
        // In real apps, we might resize here, but for now, we warn.
    }

    int u, v, w;
    int lineCount = 2; // Start tracking line numbers for error reporting
    
    while (file >> u >> v >> w) {
        // VALIDATION 1: Check for Negative Weights
        if (w < 0) {
            cerr << "[Data Error] Line " << lineCount << ": Negative weight detected. Skipping." << endl;
            continue; 
        }

        // VALIDATION 2: Boundary Checks
        if (u < 0 || u >= numVertices || v < 0 || v >= numVertices) {
            cerr << "[Data Error] Line " << lineCount << ": Node index out of bounds (" 
                 << u << "," << v << "). Max allowed: " << (numVertices - 1) << endl;
            continue;
        }

        addEdge(u, v, w);
        lineCount++;
    }
    
    // Check if loop stopped because of bad formatting (e.g., text instead of numbers)
    if (!file.eof()) {
        cerr << "[Warning] File parsing stopped early due to malformed data at line " << lineCount << endl;
    }

    file.close();
    return true;
}

// 2. Add a Road
void CityGraph::addEdge(int src, int dest, int weight) {
    if (src >= 0 && src < numVertices && dest >= 0 && dest < numVertices) {
        adjLists[src].push_back({dest, weight});
        // Since roads are two-way, add the reverse connection too
        adjLists[dest].push_back({src, weight});
    }
}

// 3. Remove a Road (Blockage simulation)
void CityGraph::removeEdge(int src, int dest) {
    // Remove connection from Src to Dest
    adjLists[src].remove_if([dest](const Edge& e) { return e.destination == dest; });
    
    // Remove connection from Dest to Src (Two-way road)
    adjLists[dest].remove_if([src](const Edge& e) { return e.destination == src; });
}

// 4. Dijkstra's Algorithm (The Shortest Path Finder)
int CityGraph::dijkstra(int startVertex, int endVertex) {
    // Priority Queue to store pairs of (distance, vertex)
    // Ordered by smallest distance first
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    // Distances initialized to Infinity
    vector<int> dist(numVertices, INT_MAX);

    // Start node distance is 0
    dist[startVertex] = 0;
    pq.push({0, startVertex});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        // If we reached the target, return the time
        if (u == endVertex) return dist[u];

        // If current distance is greater than already found shortest, skip
        if (d > dist[u]) continue;

        // Explore neighbors
        for (auto& edge : adjLists[u]) {
            int v = edge.destination;
            int weight = edge.weight;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    return -1; // If unreachable
}