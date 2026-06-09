# Project 2: ListSchedule (Processor Task Scheduling)

## 📝 Overview
This project implements a comprehensive scheduling environment for processing single-operation independent tasks on identical parallel machines (P || C_max and P || ΣC_i). It features a collection of classic heuristics, a preemptive scheduling algorithm, and an exact dynamic/backtracking solution. Additionally, the system supports real-time, dynamic modifications to the queue of tasks.

## 🧮 Implemented Scheduling Algorithms

### 1. (B) Basic List Scheduling
Processes tasks exactly in the order they arrive on the input list. Each task is dynamically assigned to the first available processor (the one with the minimum current accumulated load time).

### 2. (L) LPT (*Longest Processing Time First*)
Sorts tasks in a non-increasing order of their processing times (ties are broken lexicographically using the task ID). It then applies standard list scheduling, providing an excellent approximation guarantee for the NP-hard P || C_max minimization problem.

### 3. (S) SPT (*Shortest Processing Time First*)
Sorts tasks in a non-increasing order of execution time, then cycles them in m-sized batches across the processors. Once assigned, the tasks on each individual processor are re-sorted in an increasing order (*shortest first*). This optimally minimizes the sum of completion times.

### 4. (M) McNaughton's Algorithm
Finds the absolute optimal schedule for tasks that can be arbitrarily preempted and resumed without penalty (P | pmtn | C_max).
1. Calculates the ideal lower bound for C_max.
2. Packs tasks sequentially onto a processor until its accumulated time hits C_max.
3. If a task overflows the bound, it is split: the remainder is assigned to the next processor starting exactly at time `0`.

### 5. (A) Optimal C_max Solver (Exact Backtracking)
An exhaustive search algorithm with pruning that evaluates all valid assignment permutations. It is reserved for small problem instances to verify heuristic bounds.

## ➕ Dynamic List Modification
The task list can be altered dynamically prior to invoking a schedule algorithm:
* `+ k p` — Inserts a task of length p at the k-th position of the list. Its unique ID is calculated dynamically.
* `- i` — Deletes the task with identifier `i` from the active list, shifting subsequent elements.

## 📥 Input and Output Format
### Input
The input begins with the initial task count n, followed by n task lengths typed as `double`. Subsequent lines contain interactive commands (`+`, `-`, `B`, `L`, `S`, `M`, `A` accompanied by the processor count m).
```text
3
4.5 2.0 3.5
B 2
```

### Output
Each scheduling command prints the computed C_max and Sigma C metrics alongside a clear text map of the processor timelines:
```text
Cmax: 5.5
sigmaC: 13.5
M1:( C1 = 4.5 )
M2:( C2 = 2.0 )( C3 = 5.5 )
```
*(For McNaughton's algorithm, split tasks are denoted using `P[id] = time` to represent preemption intercepts)*.