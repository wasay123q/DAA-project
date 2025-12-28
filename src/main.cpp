#include <iostream>
#include <vector>
#include <string>
#include <limits> 
#include "../include/CityGraph.h"

using namespace std;

// --- UTILITIES ---
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void printHeader() {
    cout << "\n======================================================\n";
    cout << "                EMERGENCY SIMULATOR                  \n";
    cout << "======================================================\n";
}

int getValidInt(int min, int max, string prompt) {
    int input;
    while (true) {
        cout << prompt;
        if (cin >> input && input >= min && input <= max) {
            return input;
        } else {
            cout << "  [!] Invalid input. Enter number between " << min << "-" << max << ".\n";
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void waitForUser() {
    cout << "\n[Press Enter to return to menu...]";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    cin.get();
}

// --- MAIN ---
int main() {
    const int TOTAL_NODES = 30;
    CityGraph city(TOTAL_NODES);
    string mapFile = "data/city_map.txt";

    while (true) {
        // Reload fresh map every loop
        city = CityGraph(TOTAL_NODES); 
        if (!city.loadGraph(mapFile)) return 1;

        clearScreen();
        printHeader();
        cout << "  [1] Standard Dispatch (Normal Traffic)\n";
        cout << "  [2] Preset: Highway Collapse\n";
        cout << "  [3] Preset: Industrial Explosion\n";
        cout << "  [4] Custom Emergency Builder (With Reachability Scan)\n";
        cout << "  [5] Exit\n";
        
        int choice = getValidInt(1, 5, "\n>> Select Option: ");
        if (choice == 5) break;

        cout << "\n--- SIMULATION STARTED ---\n";
        int start, target, u, v;

        switch (choice) {
        case 1: 
            start = getValidInt(0, TOTAL_NODES - 1, "Start Sector: ");
            target = getValidInt(0, TOTAL_NODES - 1, "Target Sector: ");
            cout << ">> Travel Time: " << city.dijkstra(start, target) << " mins\n";
            break;

        case 2: 
            cout << "SCENARIO: Bridge Failure (24-25). Rerouting 0 -> 29.\n";
            city.removeEdge(24, 25);
            cout << ">> New Time: " << city.dijkstra(0, 29) << " mins\n";
            break;

        case 3: 
            cout << "SCENARIO: Toxic Leak (28). Dispatching 5 -> 28.\n";
            cout << ">> Time: " << city.dijkstra(5, 28) << " mins\n";
            break;

        case 4: // --- CUSTOM EMERGENCY WITH AUTOMATIC ZONE SCAN ---
            cout << "1. Mission Parameters:\n";
            start = getValidInt(0, TOTAL_NODES - 1, "   From Node: ");
            target = getValidInt(0, TOTAL_NODES - 1, "   To Node:   ");

            cout << "\n2. Add Obstacle:\n";
            u = getValidInt(0, TOTAL_NODES - 1, "   Block Road FROM: ");
            v = getValidInt(0, TOTAL_NODES - 1, "   Block Road TO:   ");

            city.removeEdge(u, v);
            cout << "\n>> SIMULATING BLOCKADE (" << u << " <-> " << v << ")...\n";

            {
                int time = city.dijkstra(start, target);
                
                if (time != -1 && time < 100000) {
                    // Success Case
                    cout << ">> RESULT: Route Possible. Time: " << time << " mins\n";
                } else {
                    // Failure Case - TRIGGER AUTOMATIC BFS SCAN
                    cout << ">> RESULT: CRITICAL FAILURE! Target Unreachable.\n";
                    cout << ">> DIAGNOSTIC: Scanning for Isolated Zones...\n";
                    
                    vector<int> lostNodes = city.getUnreachableNodes(start);
                    
                    cout << "   [!] The following sectors are completely CUT OFF from Sector " << start << ":\n   [ ";
                    for (int n : lostNodes) cout << n << " ";
                    cout << "]\n";
                }
            }
            break;
        }
        waitForUser();
    }
    return 0;
}