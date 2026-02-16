# 💻 Parallel and Distributed Computing (UCS645)
**Thapar Institute of Engineering and Technology**

This repository contains my laboratory work and performance evaluations for the UCS645 course, focusing on **Shared Memory Parallelism** using **OpenMP**.

---

## 📂 Repository Structure

### 🧪 [Lab 1: Fundamentals of OpenMP](./Lab1/)
Basic implementations to understand thread management and data sharing.
* **DAXPY Loop:** Measuring speedup in vector operations.
* **Matrix Multiplication:** Comparing 1D vs. 2D threading strategies.
* **PI Calculation:** Implementing the `reduction` clause for numerical integration.

### 🚀 [Lab 2: Performance Evaluation](./Lab2/)
An in-depth analysis of scalability, speedup, and efficiency across multiple threads.
* **Molecular Dynamics:** Pairwise Lennard-Jones force calculations.
* **Smith–Waterman Algorithm:** Analyzing wavefront parallelization and synchronization bottlenecks.
* **2D Heat Diffusion:** Observing superlinear speedup through cache optimization.

---

## 📊 High-Level Performance Summary (Lab 2)
![Performance Graph](Lab2/performance_graph.png)

| Application | Max Speedup | Best Thread Count | Scaling Type |
| :--- | :--- | :--- | :--- |
| Molecular Dynamics | 3.84x | 4 Threads | Near-Ideal |
| Smith-Waterman | 2.51x | 4 Threads | Low (Sync-bound) |
| Heat Diffusion | 4.13x | 4 Threads | Superlinear |

---

## 🛠️ Requirements & Setup
- **Compiler:** GCC with `-fopenmp` support.
- **Environment:** Tested on Linux/Ubuntu.
- **Execution:**
  ```bash
  g++ filename.cpp -o output -fopenmp
  ./output
