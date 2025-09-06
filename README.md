# Algorithms and Computational Complexity – Projects

This repository contains two applications developed as part of the **Algorithms and Computational Complexity** course at Wrocław University of Science and Technology.  
The projects combine **C++ (object-oriented design, main application logic)** with **C (low-level modules and data structures)**. Build configuration and compilation are handled with **CMake**.

---

## 📂 Projects

### 1. AIZO_Sorting
An application implementing and benchmarking classic **sorting algorithms**:
- Insertion Sort  
- QuickSort  
- Heap Sort  
- Shell Sort  

The program automatically generates a new random dataset for each run.  
It was designed for experimental analysis of algorithm efficiency under different input sizes and characteristics.

---

### 2. AIZO_Graphs
An application for studying the performance of **graph algorithms** depending on graph size and representation (adjacency matrix vs adjacency list).  

Implemented algorithms include:
- **Minimum Spanning Tree (MST):** Prim’s algorithm, Kruskal’s algorithm  
- **Shortest Path:** Dijkstra’s algorithm, Bellman–Ford algorithm  
- **Maximum Flow:** Ford–Fulkerson algorithm (extended version)  

Features:
- Random graph generation with configurable number of vertices and density  
- Graph loading from file (edge list format)  
- Execution of algorithms on both adjacency matrix and adjacency list representations  
- Measurement of execution times for experimental evaluation  
- Console-based menu for selecting problems and operations  

---

## 🛠 Technologies
- **C++** – object-oriented application structure, algorithm implementations  
- **C** – selected modules and low-level data handling  
- **CMake** – build configuration and project management  
- Dynamic memory allocation  
- Manual implementation of data structures (no STL/Boost)  
- Console-based interface  

---

## 📄 License
This repository was created for educational purposes as part of coursework.  
You are free to use it for study and demonstration.
