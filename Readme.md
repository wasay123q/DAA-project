# Emergency Simulator - Smart City Response System

[![Language](https://img.shields.io/badge/Language-C++-blue.svg)](https://isocpp.org/)
[![Standard](https://img.shields.io/badge/C++-11-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B11)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)

## 📋 Table of Contents
- [Overview](#overview)
- [Real-World Application](#real-world-application)
- [Technical Concepts](#technical-concepts)
- [Algorithms Implemented](#algorithms-implemented)
- [System Architecture](#system-architecture)
- [Project Structure](#project-structure)
- [Installation & Setup](#installation--setup)
- [Usage Guide](#usage-guide)
- [Simulation Scenarios](#simulation-scenarios)
- [Data Format](#data-format)
- [Features](#features)
- [Time & Space Complexity](#time--space-complexity)
- [Future Enhancements](#future-enhancements)
- [Contributing](#contributing)
- [Academic Context](#academic-context)

---

## 🌟 Overview

**Emergency Simulator** is an interactive graph-based emergency response system that simulates real-world crisis scenarios in urban environments. Built on advanced pathfinding algorithms, it calculates optimal routes for emergency vehicles, handles dynamic road blockages, and performs automatic connectivity diagnostics to identify isolated zones during emergencies.

The system features an intuitive menu-driven interface with preset crisis scenarios (Highway Collapse, Industrial Explosion) and a Custom Emergency Builder that automatically scans for unreachable zones when routes fail.

### What Problem Does It Solve?

In emergency situations, every second counts. This system addresses critical challenges faced by urban emergency services:

- **Route Optimization**: Finding the fastest path from emergency stations to incident locations
- **Dynamic Adaptability**: Recalculating routes when roads become blocked due to accidents, construction, or natural disasters
- **Connectivity Analysis**: Identifying areas that may become isolated and require alternative emergency response strategies
- **Real-time Decision Making**: Providing immediate routing solutions during crisis situations

---

## 🌍 Real-World Application

### Non-Technical Explanation

Imagine you're playing a video game where you need to navigate through a city to reach a destination. The city has:
- **Intersections** (nodes): Points where roads meet
- **Roads** (edges): Connections between intersections with different travel times
- **Obstacles**: Some roads might get blocked

This system works similarly to your GPS navigation app, but it's specifically designed for emergency services like fire trucks, ambulances, and police vehicles. When an emergency occurs:

1. The system knows where all intersections and roads are located
2. It calculates the absolute fastest route considering current traffic conditions
3. If a road gets blocked (accident, flood, etc.), it instantly finds an alternative route
4. It can identify areas that become completely cut off and need special attention

### Real-World Use Cases

| Scenario | Application |
|----------|-------------|
| **Fire Emergency** | Calculates fastest route from fire station to fire location |
| **Medical Emergency** | Directs ambulances around blocked roads to hospitals |
| **Natural Disasters** | Identifies isolated neighborhoods requiring helicopter rescue |
| **Traffic Management** | Helps police vehicles navigate through congested areas |
| **Urban Planning** | Analyzes city connectivity and identifies infrastructure weaknesses |

---

## 🔬 Technical Concepts

### Graph Theory Foundation

The system models the city as a **weighted directed graph**:

- **Vertices (Nodes)**: Represent city intersections or key locations
- **Edges**: Represent roads connecting intersections
- **Weights**: Represent travel time or distance in minutes/meters
- **Directed**: Roads may be one-way; travel from A→B doesn't guarantee B→A exists

### Graph Representation

The project uses an **Adjacency List** representation with a custom Edge structure:

```cpp
struct Edge {
    int destination;  // Target node
    int weight;       // Travel time in minutes
};

vector<list<Edge>> adjLists;  // Vector of lists for each node
```

**Example Storage**:
```
Node 0 → [(1, 3min), (5, 4min), (20, 5min)]
Node 1 → [(2, 2min), (6, 3min)]
...
```

**Why Adjacency List?**
- **Space Efficiency**: O(V + E) vs O(V²) for adjacency matrix
- **Iteration Speed**: Fast traversal of neighbors for sparse graphs
- **Real-world Fit**: Cities have sparse road networks (not every intersection connects to every other)
- **Dynamic Operations**: Efficient edge insertion and deletion using `std::list`

---

## 🧮 Algorithms Implemented

### 1. Dijkstra's Algorithm (Shortest Path)

**Purpose**: Finds the shortest path from a source node to a destination node.

**How It Works**:
1. Start at the source node with distance 0
2. Mark all other nodes as having infinite distance
3. Explore neighbors, updating their distances if a shorter path is found
4. Always select the unvisited node with the smallest known distance
5. Repeat until the destination is reached

**Algorithm Classification**: 
- **Strategy**: Greedy Algorithm
- **Paradigm**: Optimization through local best choices
- **Guarantee**: Finds globally optimal solution for non-negative weights

**Code Implementation Highlights**:
```cpp
priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
vector<int> dist(numVertices, INT_MAX);

// Early termination optimization
if (u == endVertex) return dist[u];

// Edge relaxation with weight consideration
if (dist[u] + weight < dist[v]) {
    dist[v] = dist[u] + weight;
    pq.push({dist[v], v});
}
```

**Features**:
- **Min-Heap Priority Queue**: Efficiently retrieves minimum distance node (O(log V))
- **Distance Array**: Tracks shortest known distance to each node
- **Early Termination**: Stops immediately when destination is reached
- **Returns -1**: Indicates unreachable destination

**Time Complexity**: O((V + E) log V) with binary heap priority queue  
**Space Complexity**: O(V) for distance and parent arrays

### 2. Breadth-First Search (BFS) - Connectivity Analysis

**Purpose**: Determines which nodes are reachable from a starting point.

**How It Works**:
1. Start at the source node and mark it as visited
2. Add it to a queue for processing
3. While queue is not empty:
   - Dequeue a node
   - For each neighbor, if unvisited:
     - Mark as visited
     - Add to queue
4. After BFS completes, collect all nodes that remain unvisited
5. These unvisited nodes form the isolated/unreachable zones

**Algorithm Classification**:
- **Strategy**: Graph Traversal
- **Structure**: Queue-based level-order exploration
- **Guarantee**: Finds all reachable nodes

**Time Complexity**: O(V + E)  
**Space Complexity**: O(V) for visited array and queue

---

## 🏗️ System Architecture

### Class Structure

```
┌─────────────────────────────────────────┐
│           CityGraph Class               │
├─────────────────────────────────────────┤
│ Private Members:                        │
│  - numVertices: int                     │
│  - adjLists: vector<list<Edge>>         │
├─────────────────────────────────────────┤
│ Edge Structure:                         │
│  - destination: int                     │
│  - weight: int (time in minutes)        │
├─────────────────────────────────────────┤
│ Public Methods:                         │
│  + CityGraph(vertices)                  │
│  + loadGraph(filename): bool            │
│  + addEdge(src, dest, weight)           │
│  + removeEdge(src, dest)                │
│  + dijkstra(start, end): int            │
│  + getUnreachableNodes(startNode)       │
└─────────────────────────────────────────┘
```

### Data Flow

```
          city_map.txt (30 nodes, 40 edges)
                       ↓
              loadGraph() with validation
                       ↓
              CityGraph Instance
                       ↓
         ┌─────────────┴─────────────┐
         ↓                           ↓
    Menu System              Emergency Scenarios
         ↓                           ↓
    ┌────┴────┬──────────────┬──────┴─────┐
    ↓         ↓              ↓            ↓
Standard  Highway      Industrial    Custom
Dispatch  Collapse     Explosion     Builder
    ↓         ↓              ↓            ↓
    └─────────┴──────────────┴────────────┘
                       ↓
            ┌──────────┴──────────┐
            ↓                     ↓
      dijkstra()           getUnreachableNodes()
       (Routing)           (Diagnostics)
            ↓                     ↓
     Travel Time            Isolated Zones
```

---

## 📁 Project Structure

```
DAA-project/
│
├── data/
│   └── city_map.txt          # 30-node city graph (format: u v weight)
│
├── include/
│   └── CityGraph.h           # CityGraph class and Edge struct declaration
│
├── src/
│   ├── CityGraph.cpp         # Dijkstra & BFS implementations
│   └── main.cpp              # Emergency Simulator UI & scenarios
│
└── Readme.md                 # Project documentation
```

### File Descriptions

| File | Purpose | Key Contents |
|------|---------|--------------|
| **CityGraph.h** | Interface definition | CityGraph class, Edge struct, method declarations |
| **CityGraph.cpp** | Core algorithms | Dijkstra (shortest path), BFS (connectivity), graph operations |
| **main.cpp** | Emergency Simulator | Menu system, preset scenarios, custom builder, UI utilities |
| **city_map.txt** | Graph data | 30 nodes, 40 edges (format: `node1 node2 time`) |

---

## 🚀 Installation & Setup

### Prerequisites

- **C++ Compiler**: G++ with C++11 support or later (MinGW-W64 recommended for Windows)
- **Operating System**: Windows (primary), Linux, or macOS
- **Terminal**: PowerShell, CMD, or WSL

### Step 1: Clone the Repository

```bash
git clone https://github.com/wasay123q/DAA-project.git
cd "DAA project"
```

### Step 2: Verify File Structure

Ensure all required files are present:
```bash
dir /S  # Windows
ls -R   # Linux/macOS
```

Expected structure:
```
data/city_map.txt
include/CityGraph.h
src/CityGraph.cpp, main.cpp
```

### Step 3: Compile the Project

**Option 1: Manual Compilation (Windows)**
```powershell
g++ -std=c++11 -I include -c src/CityGraph.cpp -o CityGraph.o
g++ -std=c++11 -I include -c src/main.cpp -o main.o
g++ CityGraph.o main.o -o route_planner.exe
```

**Option 2: Single Command**
```bash
g++ -std=c++11 -I include src/*.cpp -o route_planner
```

### Step 4: Run the Application

**Windows:**
```powershell
.\route_planner.exe
```

**Linux/macOS:**
```bash
./route_planner
```

### Troubleshooting

**Issue**: `g++: command not found`  
**Solution**: Install C++ compiler:
- **Windows**: Install MinGW-W64 from [winlibs.com](https://winlibs.com/) or [MSYS2](https://www.msys2.org/)
- **Ubuntu/Debian**: `sudo apt-get install build-essential`
- **macOS**: `xcode-select --install`

**Issue**: File not found errors when running  
**Solution**: Ensure you're in the project root directory and `data/city_map.txt` exists
```powershell
Test-Path "data/city_map.txt"  # Should return True
```

**Issue**: Compilation errors  
**Solution**: Verify G++ supports C++11:
```bash
g++ --version  # Should be 4.8.1 or higher
```

**Issue**: Screen doesn't clear properly  
**Solution**: This is normal behavior. The `cls` command works on Windows; on Linux/macOS it uses `clear`.

---

## 📖 Usage Guide

### Running the Emergency Simulator

When you launch the application, you'll see an interactive menu:

```
======================================================
                EMERGENCY SIMULATOR                  
======================================================
  [1] Standard Dispatch (Normal Traffic)
  [2] Preset: Highway Collapse
  [3] Preset: Industrial Explosion
  [4] Custom Emergency Builder (With Reachability Scan)
  [5] Exit

>> Select Option:
```

### Menu Options Explained

#### Option 1: Standard Dispatch
**Purpose**: Calculate optimal emergency response route under normal conditions

**User Inputs**:
- Start Sector (0-29)
- Target Sector (0-29)

**Output**:
```
>> Travel Time: 15 mins
```

**Use Case**: Quick dispatch calculations, baseline routing analysis

---

#### Option 2: Highway Collapse Preset
**Scenario**: Bridge failure between sectors 24-25, emergency dispatch from 0 → 29

**What Happens**:
1. Removes edge 24↔25 (simulates collapsed bridge)
2. Calculates alternate route from sector 0 to 29
3. Displays adjusted travel time

**Output Example**:
```
SCENARIO: Bridge Failure (24-25). Rerouting 0 -> 29.
>> New Time: 23 mins
```

**Learning Point**: Demonstrates how system handles infrastructure failures and finds alternate paths

---

#### Option 3: Industrial Explosion Preset
**Scenario**: Toxic leak at sector 28, emergency dispatch from 5 → 28

**What Happens**:
1. Calculates fastest route from station 5 to hazard zone 28
2. Displays travel time for hazmat team deployment

**Output Example**:
```
SCENARIO: Toxic Leak (28). Dispatching 5 -> 28.
>> Time: 18 mins
```

**Use Case**: Hazmat response, industrial emergency protocols

---

#### Option 4: Custom Emergency Builder ⭐
**The Most Advanced Feature**: Create your own emergency scenario with automatic diagnostics

**User Inputs**:
1. **Mission Parameters**:
   - From Node (dispatch origin)
   - To Node (emergency destination)

2. **Obstacle Definition**:
   - Block Road FROM (node U)
   - Block Road TO (node V)

**Intelligent Behavior**:

**Case A - Route Still Exists**:
```
>> SIMULATING BLOCKADE (3 <-> 8)...
>> RESULT: Route Possible. Time: 21 mins
```

**Case B - Complete Isolation (AUTO-DIAGNOSTIC)**:
```
>> SIMULATING BLOCKADE (13 <-> 14)...
>> RESULT: CRITICAL FAILURE! Target Unreachable.
>> DIAGNOSTIC: Scanning for Isolated Zones...
   [!] The following sectors are completely CUT OFF from Sector 0:
   [ 14 15 16 17 18 19 29 ]
```

**Advanced Feature**: When a route becomes impossible, the system **automatically** triggers BFS-based zone scanning to identify **all isolated sectors**, not just the target. This helps emergency coordinators:
- Allocate helicopter resources
- Identify evacuation zones
- Prioritize infrastructure repair

---

## 🎬 Simulation Scenarios

### Preset Scenario Details

| Scenario | Nodes | Blockage | Purpose |
|----------|-------|----------|---------|
| **Highway Collapse** | 0 → 29 | 24↔25 removed | Highway infrastructure failure |
| **Industrial Explosion** | 5 → 28 | None | Hazmat response timing |

### Custom Scenarios You Can Test

```cpp
// Test 1: City Center Isolation
From: 0, To: 15, Block: 13-14
Expected: Shows all east-side sectors cut off

// Test 2: Alternative Route Efficiency
From: 0, To: 10, Block: 5-10
Expected: Route via nodes 20 instead

// Test 3: Multiple Blockages (manual testing)
Run twice with different blocks to simulate cascading failures
```

### Customizing the City Map

Edit [data/city_map.txt](data/city_map.txt) to modify the graph structure:

**Format**:
```
30                    # First line: total number of nodes
u v w                 # Each subsequent line: edge from u to v with weight w
```

**Example**:
```
30                    # 30 nodes in the graph
0 1 3                 # Road from node 0 to 1 takes 3 minutes
1 2 2                 # Road from node 1 to 2 takes 2 minutes
...
```

**Important Notes**: 
- Nodes are **0-indexed** (0 through 29)
- Graph is **undirected** (edges automatically bidirectional)
- Weights must be **positive integers** (validated during load)
- Update `TOTAL_NODES` constant in [main.cpp](src/main.cpp#L44) if changing node count
- Invalid edges (negative weights, out-of-bounds nodes) are automatically ignored

### Modifying Scenarios

Edit [src/main.cpp](src/main.cpp) to customize preset scenarios:

```cpp
// Modify Highway Collapse scenario (Case 2)
city.removeEdge(24, 25);  // Change blocked road
int time = city.dijkstra(0, 29);  // Change start/end points

// Modify Industrial Explosion scenario (Case 3)
int time = city.dijkstra(5, 28);  // Adjust dispatch parameters
```

---

## 📊 Data Format

### Input File Structure (city_map.txt)

The city map represents a **30-node urban network** with 40 bidirectional roads:

**File Structure**:
```
30              # Header: Total number of nodes
0 1 3           # Edge: Node 0 ↔ Node 1, 3 minutes
0 5 4           # Edge: Node 0 ↔ Node 5, 4 minutes
...
28 29 5         # Edge: Node 28 ↔ Node 29, 5 minutes
```

### City Layout Overview

The graph simulates a realistic urban network with:
- **Main Highway**: Nodes 0→20→21→22→23→24→25→26→27→28→29 (10 nodes)
- **City Grid**: Nodes 0-19 (dense interconnected grid)
- **Bypass Routes**: Multiple alternate paths for redundancy
- **Critical Junctions**: Nodes 0, 10, 20 (high connectivity)

**Visualization Excerpt**:
```
       3min   2min   2min
    0 ──→ 1 ──→ 2 ──→ 3 ──→ 4
    │4    │3    │5    │4    │3
    ↓     ↓     ↓     ↓     ↓
    5 ──→ 6 ──→ 7 ──→ 8 ──→ 9
   2min        2min        2min
    ...
  (Pattern continues to node 29)
```

### Edge List (Sample)

| From | To | Time | Description |
|------|----|----|-------------|
| 0 | 1 | 3 min | Main arterial road |
| 0 | 5 | 4 min | Vertical connector |
| 0 | 20 | 5 min | Highway on-ramp |
| 24 | 25 | 5 min | **Critical bridge** (Highway Collapse scenario) |
| 5 | 28 | - | **Not connected** (requires multiple hops) |
| 10 | 20 | 15 min | Long bypass route |

### Graph Properties

- **Nodes (V)**: 30
- **Edges (E)**: 40 (undirected, so 80 directed edges internally)
- **Connectivity**: Fully connected under normal conditions
- **Diameter**: ~8-10 hops (longest shortest path)
- **Average Degree**: 2.67 edges per node (sparse graph)
- **Weight Range**: 2-15 minutes per edge

---

## ✨ Features

### ✅ Core Functionality

- [x] **Interactive Menu System**: 5-option menu with input validation
- [x] **Shortest Path Calculation**: Dijkstra's algorithm with early termination
- [x] **Dynamic Road Blockage**: Runtime edge removal for emergency simulation
- [x] **Connectivity Analysis**: BFS-based unreachable zone detection
- [x] **Preset Crisis Scenarios**: Highway collapse and industrial explosion simulations
- [x] **Custom Emergency Builder**: User-defined scenarios with auto-diagnostics
- [x] **Automatic Zone Scanning**: Triggers BFS when routes fail
- [x] **Input Validation**: Robust error handling for user inputs
- [x] **Cross-Platform UI**: Screen clearing for Windows/Linux/macOS
- [x] **Graph Validation**: Ignores invalid edges during file loading
- [x] **Fresh State per Simulation**: Reloads clean graph for each scenario

### 🔧 Technical Features

- **Undirected Graph**: Bidirectional roads (automatic reverse edge creation)
- **Weighted Edges**: Time-based routing optimization
- **Sparse Graph Representation**: Adjacency list with `std::list<Edge>`
- **Early Path Termination**: Dijkstra stops at destination, not all nodes
- **Unreachability Detection**: Returns -1 for impossible routes
- **Memory-Efficient BFS**: Manual queue implementation using vector

### 📈 Performance Characteristics

| Operation | Time Complexity | Space Complexity |
|-----------|----------------|------------------|
| Graph Loading | O(E) | O(V + E) |
| Add Edge | O(1) | O(1) |
| Remove Edge | O(deg(v)) | O(1) |
| Shortest Path (Dijkstra) | O((V+E) log V) | O(V) |
| Connectivity Check (BFS) | O(V + E) | O(V) |
| Full Simulation Cycle | O((V+E) log V) | O(V + E) |

*deg(v) = degree of vertex (average ~3 for this graph)*

---

## ⏱️ Time & Space Complexity

### Dijkstra's Algorithm Analysis

**Time Complexity**: O((V + E) log V)
- **Explanation**:
  - Each vertex is extracted from priority queue once: O(V log V)
  - Each edge is relaxed once: O(E log V)
  - Total: O((V + E) log V)

**Space Complexity**: O(V)
- Distance array: O(V)
- Parent array: O(V)
- Priority queue: O(V) in worst case

### BFS Algorithm Analysis

**Time Complexity**: O(V + E)
- **Explanation**:
  - Each vertex is visited once: O(V)
  - Each edge is examined once: O(E)
  - Total: O(V + E)

**Space Complexity**: O(V)
- Visited array: O(V)
- Queue: O(V) in worst case

### Real-World Performance

For a typical city with:
- 1,000 intersections (V = 1,000)
- 3,000 roads (E = 3,000)

**Dijkstra's**: ~0.5 ms (fast enough for real-time use)  
**BFS**: ~0.2 ms (extremely fast connectivity checks)

---

## 🔮 Future Enhancements

### Planned Features

#### 1. **Path Visualization**
- Display actual route taken (node sequence)
- Show alternate paths with their costs
- Implementation: Return path vector from Dijkstra

#### 2. **Multi-Source Dijkstra**
- Find nearest emergency station from incident location
- Dispatch from multiple stations simultaneously
- Implementation: Reverse graph or multiple Dijkstra runs

#### 3. **Real-Time Weight Updates**
- Dynamic traffic condition simulation
- Time-based edge weight variations (rush hour modeling)
- Implementation: Update edge weights during runtime

#### 4. **Persistent Blockages**
- Save and load blocked roads between sessions
- Scenario management system
- Implementation: Blockage state file (JSON/XML)

#### 5. **Graphical Visualization**
- Interactive city map display
- Animated route highlighting
- Technology stack: SDL2, SFML, or web-based (D3.js)

#### 6. **Advanced Algorithms**
- **A* Algorithm**: Heuristic-based faster pathfinding
- **Floyd-Warshall**: All-pairs shortest paths pre-computation
- **Bellman-Ford**: Support for negative weights (traffic delays)

#### 7. **Multi-Criteria Optimization**
- Balance time, distance, safety scores
- Weighted objective function
- Pareto frontier for multi-objective solutions

#### 8. **Statistical Analysis Module**
- Average response time calculations
- Most critical edges/nodes identification
- Network vulnerability assessment

#### 9. **Batch Scenario Testing**
- Load scenarios from file
- Automated testing suite
- Performance benchmarking

#### 10. **Enhanced Diagnostics**
- Show why routes fail (which segments are cut)
- Suggest minimum edges to restore connectivity
- Implementation: Cut-edge/articulation point detection

---

## 🤝 Contributing

### How to Contribute

1. **Fork the Repository**
2. **Create a Feature Branch**
   ```bash
   git checkout -b feature/YourFeatureName
   ```
3. **Commit Your Changes**
   ```bash
   git commit -m "Add: Brief description of changes"
   ```
4. **Push to Your Fork**
   ```bash
   git push origin feature/YourFeatureName
   ```
5. **Open a Pull Request**

### Contribution Guidelines

- Follow existing code style and naming conventions
- Add comments for complex algorithms
- Update README if adding new features
- Test thoroughly before submitting

---

## 🎓 Academic Context

### Course Information

- **Course**: Design and Analysis of Algorithms (DAA)
- **Level**: 5th Semester, Bachelor of Science in Computer Science
- **Learning Outcomes**:
  - **CLO3**: Apply greedy strategies for optimization problems
  - Graph algorithm implementation and analysis
  - Time complexity analysis of algorithms
  - Real-world problem-solving using algorithmic approaches

### Algorithm Coverage

| Algorithm | Type | CLO | Page Reference |
|-----------|------|-----|----------------|
| Dijkstra's | Greedy | CLO3 | [cite: 3] |
| BFS | Graph Traversal | CLO1 | [cite: 28] |
| Dynamic Graph Ops | Data Structures | CLO2 | [cite: 27] |

### Key Concepts Demonstrated

1. **Graph Representation**: Adjacency list vs. adjacency matrix
2. **Greedy Algorithms**: Local optimization leading to global optimum
3. **Priority Queues**: Efficient minimum extraction for Dijkstra's
4. **Path Reconstruction**: Backtracking through parent pointers
5. **Graph Traversal**: BFS for connectivity analysis
6. **Algorithm Analysis**: Big-O notation and complexity analysis
7. **Real-world Modeling**: Abstraction of city networks as graphs

---

## � License

This project is an academic assignment for educational purposes. Feel free to use and modify for learning.

---

## 👨‍💻 Author

**Wasay Ahmad**
- GitHub: [@wasay123q](https://github.com/wasay123q)
- Project Repository: [DAA-project](https://github.com/wasay123q/DAA-project)
- Course: Design and Analysis of Algorithms (5th Semester BSCS)

---

## 🙏 Acknowledgments

- **Course Instructors** for algorithm concepts and project guidance
- **CLRS** (*Introduction to Algorithms*) for Dijkstra and BFS theory
- **Data Structures & Algorithms** course materials
- Real-world emergency response systems for inspiration

---

## 📞 Support & Contact

### Getting Help

- **Issues**: Open an issue on [GitHub Issues](https://github.com/wasay123q/DAA-project/issues)
- **Documentation**: Review this README and inline code comments
- **Algorithm Theory**: Consult CLRS textbook or course materials

### Reporting Bugs

When reporting issues, please include:
1. Your operating system and compiler version
2. Exact error message or unexpected behavior
3. Steps to reproduce the problem
4. Relevant code snippets or input data

---

**Built for emergency response optimization and algorithmic learning 🚒🚑🚓**

*Last Updated: December 2025*
