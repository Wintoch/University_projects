# Project 1: TreapHouse (Product Database via Cartesian Tree)

## 📝 Overview
This project implements a highly efficient product database management system where each product is defined by a unique integer identifier (`ID`) and an integer price. To meet rigorous execution time limits under heavy transaction volumes, the underlying data structure is a non-rotational **Cartesian Tree (Treap)**. Standard BSTs are insufficient due to the risk of degenerating into a linear chain when processing pre-sorted inputs.

## 🌲 Core Structure: The Treap
A Treap is a randomized hybrid of a Binary Search Tree (BST) and a Heap. Each node maintains:
1. **Key (ID):** Follows the standard BST property (keys in the left subtree are smaller, keys in the right subtree are larger than the parent's key).
2. **Priority:** A randomly assigned floating-point value generated upon insertion. It follows the Max-Heap property (a parent's priority is always greater than its children's priorities).

This randomization ensures with high probability that the tree remains balanced, maintaining an expected height of O(log n).

### Non-Rotational Mechanics (Split / Merge)
To retain both structural invariants simultaneously without complex tree rotations, the implementation relies entirely on two primitives:
* **`Split(Treap t, int key, Treap &l, Treap &r)`:** Severs the tree into two disjoint subtrees: L (all keys <= `key`) and R (all keys > `key`).
* **`Merge(Treap &t, Treap l, Treap r)`:** Combines two subtrees into a single valid Treap based on priority hierarchies.

Nodes are augmented with metadata fields: `size` (total node count in the subtree) and `sum` (total price of all nodes in the subtree). These are maintained in O(1) time during structural updates.

## 🛠️ Supported Operations
* `A id cena` (**Add**): Inserts a product. If the `ID` already exists, it is first purged and re-inserted with a newly generated random priority to update its price.
* `D id` (**Delete**): Removes a product by isolating its node via splitting, dropping it, and merging the remaining subtrees. Non-existent IDs are safely ignored.
* `S id_min id_max` (**Sum**): Computes the exact sum of prices for all items within the closed interval [id_min, id_max] in O(log n) time by performing a double-split sequence.
* `C` (**Count**): Returns the total number of products currently stored in O(1) time by querying the cached `size` field at the root node.

## 📥 Input and Output Format
### Input
The first line contains the total number of operations N. The following lines contain sequential commands:
```text
4
A 10 50
A 20 100
S 10 15
C
```

### Output
For each `S` and `C` operation, the program outputs the result on a new line:
```text
50
2
```