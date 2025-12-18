#include <iostream>
#include <fstream>
#include <vector>
#include "../include/CityGraph.h"

// Helper to load data
CityGraph loadCityData(const std::string& filename, int& maxNodes) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        exit(1);
    }

    // First pass: Determine number of nodes (assuming 0-indexed)
    // In a real app, you might read this from the first line.
    // For this demo, we assume max node ID 5 -> 6 nodes.
    maxNodes = 6; 
    CityGraph city(maxNodes);

    int u, v, w;
    while (file >> u >> v >> w) {
        city.addRoad(u, v, w);
    }
    
    file.close();
    std::cout << "Data loaded successfully from " << filename << std::endl;
    return city;
}

void printRoute(int cost, const std::vector<int>& path) {
    if (cost == -1) {
        std::cout << " >> ALERT: Destination is unreachable!\n";
    } else {
        std::cout << " >> Time Cost: " << cost << " mins\n";
        std::cout << " >> Route: ";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i] << (i < path.size() - 1 ? " -> " : "");
        }
        std::cout << "\n";
    }
}

int main() {
    std::string dataFile = "data/city_map.txt";
    int numNodes = 0;
    
    // 1. Initialize System
    std::cout << "=== Smart City Emergency Response System ===\n";
    CityGraph city = loadCityData(dataFile, numNodes);
    city.printGraph();

    // Scenario Setup
    int station = 0; // Fire Station
    int accidentSite = 5; // Accident Location

    // 2. Normal Conditions Response [cite: 27]
    std::cout << "\n[CASE 1] Normal Traffic Conditions:\n";
    std::vector<int> path;
    int normalTime = city.getShortestPath(station, accidentSite, path);
    printRoute(normalTime, path);

    // 3. Simulate Blockage (e.g., Accident on road 2->3) 
    std::cout << "\n[CASE 2] Simulating Road Blockage (Node 2 -> Node 3)...\n";
    city.removeRoad(2, 3);
    
    int blockedTime = city.getShortestPath(station, accidentSite, path);
    printRoute(blockedTime, path);

    // 4. Comparison Analysis
    if (blockedTime != -1 && normalTime != -1) {
        std::cout << "\n>> ANALYSIS: Delay caused by blockage: " 
                  << (blockedTime - normalTime) << " mins\n";
    }

    // 5. Unreachable Zones Check [cite: 28]
    std::cout << "\n[CASE 3] Checking for Isolated Zones...\n";
    // Let's block the only entrance to Node 4 to test this
    city.removeRoad(2, 4); 
    city.removeRoad(3, 4);
    
    std::vector<int> isolated = city.getUnreachableNodes(station);
    if (!isolated.empty()) {
        std::cout << " >> WARNING: The following zones are unreachable from Station " << station << ": ";
        for (int node : isolated) std::cout << node << " ";
        std::cout << "\n";
    } else {
        std::cout << " >> All zones are reachable.\n";
    }

    return 0;
}