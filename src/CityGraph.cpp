#include "../include/CityGraph.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <climits> 

using namespace std;

// Constructor
CityGraph::CityGraph(int vertices) {
    this->numVertices = vertices;
    adjLists.resize(vertices);
}

// 1. Load Map from File (With Industrial Validation)
bool CityGraph::loadGraph(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "[Error] File not found: " << filename << endl;
        return false;
    }

    int fileNodeCount;
    if (!(file >> fileNodeCount)) {
        cerr << "[Error] Map file header missing." << endl;
        return false;
    }

    int u, v, w;
    while (file >> u >> v >> w) {
        // Validation: Ignore negative weights or out-of-bounds nodes
        if (w < 0 || u < 0 || u >= numVertices || v < 0 || v >= numVertices) {
            continue; 
        }
        addEdge(u, v, w);
    }
    
    file.close();
    return true;
}

// 2. Add Road
void CityGraph::addEdge(int src, int dest, int weight) {
    adjLists[src].push_back({dest, weight});
    adjLists[dest].push_back({src, weight}); // Undirected graph
}

// 3. Remove Road
void CityGraph::removeEdge(int src, int dest) {
    adjLists[src].remove_if([dest](const Edge& e) { return e.destination == dest; });
    adjLists[dest].remove_if([src](const Edge& e) { return e.destination == src; });
}

// 4. Dijkstra's Algorithm (Shortest Path)
int CityGraph::dijkstra(int startVertex, int endVertex) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> dist(numVertices, INT_MAX);

    dist[startVertex] = 0;
    pq.push({0, startVertex});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (u == endVertex) return dist[u];
        if (d > dist[u]) continue;

        for (auto& edge : adjLists[u]) {
            int v = edge.destination;
            int weight = edge.weight;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    return -1; // -1 represents Unreachable
}

// 5. BFS Algorithm (Finds "Unidentifiable"/Unreachable Nodes)
vector<int> CityGraph::getUnreachableNodes(int startNode) {
    vector<bool> visited(numVertices, false);
    vector<int> queue; 
    
    // Start BFS
    visited[startNode] = true;
    queue.push_back(startNode);

    int head = 0;
    while(head < queue.size()){
        int u = queue[head++];
        for(const auto& edge : adjLists[u]) {
            if(!visited[edge.destination]) {
                visited[edge.destination] = true;
                queue.push_back(edge.destination);
            }
        }
    }

    // Collect Isolated Nodes
    vector<int> isolatedNodes;
    for(int i = 0; i < numVertices; i++) {
        if(!visited[i]) {
            isolatedNodes.push_back(i);
        }
    }
    return isolatedNodes;
}