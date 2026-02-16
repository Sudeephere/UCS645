# 💻 Parallel and Distributed Computing (UCS645)
**Thapar Institute of Engineering and Technology**

This repository documents high-performance implementations and scalability evaluations using **OpenMP**. The project progresses from basic shared-memory concepts to advanced SIMD vectorization and algorithmic optimizations.

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

### ⚡ [Lab 3: Optimized Correlation Matrix](./Lab3/)
[cite_start]High-performance Pearson correlation computation for large matrices using an $O(N^2)$ pre-normalization strategy. [cite: 3]
* [cite_start]**Algorithmic Optimization:** Reduced correlation to a simple Dot Product through matrix pre-normalization. [cite: 3]
* [cite_start]**Hardware-Level Parallelism:** Leveraged `collapse(2)` and `omp simd` for peak throughput. [cite: 3]
* [cite_start]**Benchmarking:** Achieved a peak processing rate of **16.97 GFLOPS**. [cite: 3]

---

## 📊 Unified Performance Summary

| Application | Peak Speedup | Peak GFLOPS | Best Thread Count | Scaling Characteristic |
| :--- | :--- | :--- | :--- | :--- |
| **Heat Diffusion** | 4.13x | - | 4 Threads | **Superlinear** (Cache-driven) |
| **Molecular Dynamics**| 3.84x | - | 4 Threads | Near-Ideal |
| **Correlation Matrix**| 3.73x | **16.97** | 4 Threads | **Compute-Bound** (SIMD-ready) |
| **Smith-Waterman** | 2.51x | - | 4 Threads | Low (Sync-bottleneck) |

---

## 🏁 Key Engineering Insights

### 1. The 4-Thread "Sweet Spot"
[cite_start]Across all experiments (Lab 2 & 3), performance consistently saturated or degraded beyond 4 threads. [cite: 3] [cite_start]This indicates that on the quad-core test environment, oversubscription to 8 threads leads to memory bandwidth contention and Hyper-threading (SMT) overhead. [cite: 3]

### 2. Algorithmic Pre-Normalization
[cite_start]In Lab 3, instead of standard correlation, we pre-normalized the matrix (mean=0, std\_dev=1). [cite: 3] [cite_start]This transformed the hot loop into a streamlined dot product, maximizing SIMD efficiency. [cite: 3]

### 3. Cache Locality & Superlinear Speedup
[cite_start]Experiment 3 (Heat Diffusion) showed efficiency $>100\%$ at 2 and 4 threads. [cite: 3] [cite_start]This is a classic case where the distributed data sub-segments fit entirely into the L3 cache, reducing main memory latency. [cite: 3]

---

## 🛠️ Requirements & Execution
- **Compiler:** GCC 11.4.0+ with `-fopenmp` and `-O3`.
- **Optimization Flags:** `-O3 -ffast-math -march=native`.
- **Command:**
  ```bash
  g++ -O3 -fopenmp filename.cpp -o output
  ./output
