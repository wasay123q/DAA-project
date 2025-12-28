#include <iostream>
#include <vector>
#include <string>
#include <limits> // Required for input sanitization
#include "../include/CityGraph.h"

using namespace std;

// --- UTILITY: Cross-Platform Clear Screen ---
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// --- VALIDATION HELPER: The "Industrial Gatekeeper" ---
// This function refuses to return until the user gives a valid integer in range.
int getValidInt(int min, int max, string prompt) {
    int input;
    while (true) {
        cout << prompt;
        if (cin >> input) {
            // Check boundaries
            if (input >= min && input <= max) {
                // Success! Return the value.
                return input;
            } else {
                cout << "  [!] Error: Input must be between " << min << " and " << max << ".\n";
            }
        } else {
            // Check for data type mismatch (e.g., user typed "abc")
            cout << "  [!] Error: Invalid input type. Please enter a number.\n";
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
        }
    }
}

void waitForUser() {
    cout << "\n[Press Enter to return to menu...]";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ensure buffer is clear
    cin.get();
}

void printHeader() {
    cout << "\n======================================================\n";
    cout << "               EMERGENCY SIMULATOR                  \n";
    cout << "======================================================\n";
}

int main() {
    const int TOTAL_NODES = 30;
    CityGraph city(TOTAL_NODES);
    string mapFile = "data/city_map.txt";
    int choice = 0;

    while (true) {
        // RESET STATE: Reload graph fresh every iteration
        city = CityGraph(TOTAL_NODES); 
        if (!city.loadGraph(mapFile)) {
            cerr << "CRITICAL FAILURE: System halted due to map load error.\n";
            return 1;
        }

        clearScreen();
        printHeader();
        cout << "Select a Simulation Scenario:\n\n";
        cout << "  [1] Standard Dispatch (Normal Traffic)\n";
        cout << "  [2] Scenario: Highway Collapse (Preset)\n";
        cout << "  [3] Scenario: Industrial Explosion (Preset)\n";
        cout << "  [4] Custom Emergency (User Defined)\n";
        cout << "  [5] Exit Simulator\n";
        
        // VALIDATION: Menu Selection (1-5 only)
        cout << endl;
        choice = getValidInt(1, 5, ">> Enter Option: ");

        if (choice == 5) {
            cout << "Shutting down system..." << endl;
            break;
        }

        cout << "\n--- SIMULATION STARTED ---\n";

        int start, target, u, v;

        switch (choice) {
        case 1: // Standard
            start = getValidInt(0, TOTAL_NODES - 1, "Enter Start Sector (0-29): ");
            // VALIDATION: Prevent Start == Target
            do {
                target = getValidInt(0, TOTAL_NODES - 1, "Enter Target Sector (0-29): ");
                if (start == target) cout << "  [!] Error: Target cannot be the same as Start.\n";
            } while (start == target);

            {
                cout << "\nCalculating fastest route...\n";
                int time = city.dijkstra(start, target);
                cout << ">> Estimated Arrival Time: " << time << " mins" << endl;
            }
            break;

        case 2: // Highway Collapse
            cout << "SCENARIO: Highway Bridge Failure (Nodes 24-25).\n";
            city.removeEdge(24, 25);
            
            {
                int time = city.dijkstra(0, 29); // HQ to Industrial
                cout << ">> Rerouted Time: " << time << " mins\n";
            }
            break;

        case 3: // Industrial Explosion
            cout << "SCENARIO: Toxic Leak at Node 28.\n";
            {
                int time = city.dijkstra(5, 28); // Hospital to Industrial
                cout << ">> Emergency Response Time: " << time << " mins\n";
            }
            break;

        case 4: // Custom Mode
            cout << "1. Define Route:\n";
            start = getValidInt(0, TOTAL_NODES - 1, "   From Sector: ");
            target = getValidInt(0, TOTAL_NODES - 1, "   To Sector:   ");

            cout << "\n2. Define Blockage:\n";
            u = getValidInt(0, TOTAL_NODES - 1, "   Block Road FROM: ");
            
            // VALIDATION: Cannot block connection to self
            do {
                v = getValidInt(0, TOTAL_NODES - 1, "   Block Road TO:   ");
                if (u == v) cout << "  [!] Error: A road must connect two different nodes.\n";
            } while (u == v);

            city.removeEdge(u, v);
            cout << "\n>> Simulating Road Closure (" << u << " <-> " << v << ")... \n";
            
            {
                int time = city.dijkstra(start, target);
                if (time == -1 || time > 100000) 
                    cout << ">> RESULT: Target is UNREACHABLE with current blockages.\n";
                else 
                    cout << ">> RESULT: New Route Time: " << time << " mins\n";
            }
            break;
        }

        waitForUser();
    }

    return 0;
}