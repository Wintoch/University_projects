# Algorithms and Data Structures (A&DS)

This repository contains advanced algorithmic projects developed as part of the **Algorithms and Data Structures** course. Each project focuses on a distinct class of computational problems, utilizing highly optimized data structures, parallel machine scheduling techniques, and classic graph theory algorithms.

---

## 📂 Repository Structure

The repository is organized into three main project subfolders:

### 🔹 [proj1](./proj1) — TreapHouse (Product Database via Cartesian Tree)
* **Description:** A high-performance product database management system (storing ID and price) supporting dynamic range queries in logarithmic time.
* **Data Structure:** Cartesian Tree (commonly known as a **Treap**) — a hybrid of a Binary Search Tree (BST) and a Max-Heap, implemented using non-rotational `Split` and `Merge` operations.
* **Key Features:** Logarithmic insertions, updates, and deletions; O(log n) range sum queries for closed intervals [ID_min, ID_max]; and O(1) tracked size maintenance.

### 🔹 [proj2](./proj2) — ListSchedule (Processor Task Scheduling)
* **Description:** A comprehensive scheduling suite for single-operation tasks on identical parallel machines minimizing either maximum completion time (P || C_max) or total flow time (P || Sigma C_i).
* **Implemented Algorithms:** * Basic List Scheduling (B)
  * LPT (*Longest Processing Time First*) (L)
  * SPT (*Shortest Processing Time First*) with cyclic distribution (S)
  * McNaughton's Algorithm for preemptive scheduling (P | pmtn | C_max) (M)
  * Exact Backtracking Algorithm for finding the absolute optimal C_opt (A)
* **Key Features:** Dynamic real-time modification of the task queue (arbitrary positional insertion, targeted deletion by ID) paired with descriptive schedule outputs.

### 🔹 [proj3](./proj3) — Matchmaker / Swatka (Maximum Graph Matching)
* **Description:** An optimization system designed to find maximum matching configurations (pairing individuals based on reciprocal preferences and relationship weights).
* **Implemented Algorithms:**
  * **Hungarian Method (Kuhn-Munkres):** For weighted bipartite graphs — a matrix-based optimization utilizing row/column reductions, dual variables (potentials), and slack arrays.
  * **Edmonds' Blossom Algorithm:** For unweighted general graphs — a BFS-based approach that dynamically collapses odd-length cycles (blossoms) to preserve the alternating path structure.
  * **Exhaustive Backtracking (Weighted Brut):** An exact solver for weighted general graphs.
* **Key Features:** Autonomous graph structure detection (BFS bipartiteness coloring and weight homogeneity validation) to dynamically deploy the most mathematically optimal solver.

---

## 🛠️ Compilation and Execution

All programs are written in **C** and **C++** with a strict focus on memory efficiency and execution speed (no external libraries used, pure custom dynamic structures).

### Compilation Commands:
```bash
# Project 1: TreapHouse (C++)
g++ -O3 proj1/treap.cpp -o proj1/treap

# Project 2: ListSchedule (C)
gcc -O3 proj2/ListSchedule.c -o proj2/listschedule

# Project 3: Matchmaker (C)
gcc -O3 proj3/pair3test.c -o proj3/matchmaker