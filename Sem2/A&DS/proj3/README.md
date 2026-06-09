# Project 3: Matchmaker / Swatka (Maximum Graph Matching)

## 📝 Overview
This project solves the maximum matching problem (pairing elements to maximize total relationship weight) across diverse social preference models. It supports both bipartite configurations (e.g., discrete groups like genders) and general graph configurations (arbitrary attraction structures).

## 🧠 Autonomous Solver Selection Architecture
The system parses the input graph and executes a two-tiered pre-validation routine:
1. `checkIfBipartiate(graph)` — Performs a Breadth-First Search (BFS) graph coloring routine to verify two-colorability.
2. `checkIfUnitGraph(graph)` — Examines edge weights to determine if the instance is unweighted (all weights equal 1).

Based on the topological classification, the program autonomously routes the data to the most efficient mathematical engine:

### ⚙️ Engine 1: Bipartite Weighted Graphs -> Hungarian Algorithm
If the graph is verified as bipartite, the problem maps onto maximum weighted bipartite matching. The program deploys an optimized matrix-based **Hungarian Method (Kuhn-Munkres)**:
* **Cost Conversion:** Identifies local row maximum values to convert the objective from maximization to cost minimization, executing a row-column reduction.
* **Dual Variables & Slack Arrays:** Maintains dual potential vectors `u` and `v` along with a tracking array `slack`. This allows the dual variables to adjust dynamically, shifting bounds to expose new zeroes without costly full-matrix recalculations.

### ⚙️ Engine 2: General Unweighted Graphs -> Edmonds' Blossom Algorithm
If the graph contains odd-length cycles (violating bipartiteness) but features uniform unit weights, the system invokes **Edmonds' Blossom Shrinking Algorithm**:
* **Blossom Detection:** During an alternating-tree BFS, an edge connecting two active *Outer* (Type 1) nodes reveals an odd cycle (a blossom).
* **Compression (`shrink`):** Traces parent pointers to find the Lowest Common Ancestor (LCA), which acts as the blossom's base. The entire loop is compressed into a single virtual macro-node, and internal *Inner* nodes (Type 2) are upgraded to *Outer* nodes and queued.
* **Implicit Path Lifting:** During augmentation, the algorithm seamlessly traces back through the compressed structures. Because `shrink` transforms the parent pointers into a bi-directional circuit, the reversal loop naturally traverses the even-length path of the blossom.

### ⚙️ Engine 3: General Weighted Graphs -> Exhaustive Search
For complex weighted general graphs with odd cycles, the program runs an optimized **Backtracking Solver (Weighted Brut)** to guarantee the exact maximum matching configuration for compact community profiles.

## 📥 Input and Output Format
### Input
The first line states the number of problem instances t. Each instance defines vertex count n and edge count m, followed by m lines of triples `u v w` (node u, node v, edge weight w).
```text
1
4 4
1 3 1
1 4 4
2 3 3
2 4 2
```

### Output
For each instance, the program prints a single integer representing the maximum possible aggregate weight of the matching:
```text
7
```

## 📂 Test Data & Verification
The project directory includes a pre-configured `data/` folder containing ready-to-use sample data to verify the program's input and output correctness:
* `data/test.in` — Standard input file containing sample graph configurations (test instances).
* `data/test.out` — Expected standard output containing the correct maximum matching results for validation.