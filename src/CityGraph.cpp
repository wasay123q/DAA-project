#include "../include/CityGraph.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <set>

// Constructor
CityGraph::CityGraph(int nodes) : numNodes(nodes) {
    adj.resize(nodes);
}

// Add a directed weighted edge
void CityGraph::addRoad(int u, int v, int weight) {
    if (u >= 0 && u < numNodes && v >= 0 && v < numNodes) {
        adj[u].push_back({v, weight});
    }
}

// Simulate road blockage by removing an edge
void CityGraph::removeRoad(int u, int v) {
    if (u < numNodes) {
        // Use a lambda to find and remove the specific edge
        auto& edges = adj[u];
        for (auto it = edges.begin(); it != edges.end(); ++it) {
            if (it->first == v) {
                edges.erase(it);
                break; // Assuming single directed edge per pair
            }
        }
    }
}

// Dijkstra's Algorithm Implementation
// Returns total time cost, fills 'path' vector with the route
int CityGraph::getShortestPath(int start, int end, std::vector<int>& path) {
    // Priority queue stores {current_dist, u}
    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> pq;
    
    std::vector<int> dist(numNodes, INF);
    std::vector<int> parent(numNodes, -1);

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        // Optimization: If we found the destination, stop early
        if (u == end) break;
        if (d > dist[u]) continue;

        for (auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // Reconstruct path
    path.clear();
    if (dist[end] == INF) return -1; // Path not found

    for (int v = end; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    std::reverse(path.begin(), path.end());

    return dist[end];
}

// Identify unreachable zones using BFS
std::vector<int> CityGraph::getUnreachableNodes(int startNode) {
    std::vector<bool> visited(numNodes, false);
    std::queue<int> q;

    visited[startNode] = true;
    q.push(startNode);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (auto& edge : adj[u]) {
            int v = edge.first;
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    // Collect all unvisited nodes
    std::vector<int> unreachable;
    for (int i = 0; i < numNodes; ++i) {
        if (!visited[i]) {
            unreachable.push_back(i);
        }
    }
    return unreachable;
}

void CityGraph::printGraph() const {
    std::cout << "\n--- Current City Map Configuration ---\n";
    for (int i = 0; i < numNodes; ++i) {
        std::cout << "Intersection " << i << " connects to: ";
        for (const auto& edge : adj[i]) {
            std::cout << "[Node " << edge.first << " | " << edge.second << "m] ";
        }
        std::cout << "\n";
    }
    std::cout << "--------------------------------------\n";
}