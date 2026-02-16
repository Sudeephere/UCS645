# 🚀 OpenMP Performance Evaluation
## UCS645: Parallel and Distributed Computing | Lab Assignment 2

This repository contains a performance analysis of three distinct computational problems parallelized using OpenMP. The goal is to evaluate scalability, speedup, and efficiency across varying thread counts on a multi-core architecture.

---

## 🛠 Experimental Setup
- **Language:** C/C++ with OpenMP API
- **Hardware Environment:** Quad-core physical environment (Performance drops observed at 8 threads due to oversubscription).
- **Key Metrics:**
  - **Speedup ($S$):** $T_{serial} / T_{parallel}$
  - **Efficiency ($E$):** $S / \text{number of threads}$

---

## 🔬 Experiment 1: Molecular Dynamics (Lennard-Jones)
**Objective:** Compute pairwise forces and potential energy for a 3D particle system.

### Implementation Strategy
- **Parallelism:** Outer loop distribution.
- **Synchronization:** `atomic` updates for force vectors to prevent race conditions.
- **Reduction:** Total energy aggregated using the `reduction(+:energy)` clause.
- **Scheduling:** `dynamic` to mitigate load imbalance.

### 📊 Performance Results
| Threads | Execution Time (s) | Speedup (S) | Efficiency (E) |
| :--- | :--- | :--- | :--- |
| 1 | 0.1039 | 1.00x | 100% |
| 2 | 0.0539 | 1.93x | 96.4% |
| 4 | 0.0271 | 3.84x | 96.0% |
| 8 | 0.0276 | 3.77x | 47.1% |

---

## 🧬 Experiment 2: Smith–Waterman DNA Alignment
**Objective:** Local sequence alignment using dynamic programming on a $10,000 \times 10,000$ matrix.

### 🧩 The Challenge: Data Dependency
Unlike the other experiments, this uses **Wavefront (Anti-diagonal) Parallelization**. Each cell $H(i, j)$ depends on its top, left, and diagonal neighbors.

### 📊 Performance Results
| Threads | Execution Time (s) | Speedup (S) | Efficiency (E) |
| :--- | :--- | :--- | :--- |
| 1 | 6.9247 | 1.00x | 100% |
| 2 | 4.0375 | 1.71x | 85.7% |
| 4 | 2.7481 | 2.51x | 62.9% |
| 8 | 7.6440 | 0.90x | 11.3% |

> **Note:** Performance severely degrades at 8 threads because the synchronization overhead after each diagonal computation outweighs the gains when threads exceed physical cores.

---

## 🌡️ Experiment 3: 2D Heat Diffusion
**Objective:** Simulate heat transfer across a $2000 \times 2000$ grid over 500 time steps.

### 🔥 Implementation Highlights
- **Domain Decomposition:** Grid points distributed across threads.
- **Race Conditions:** None during write-back as threads access distinct memory segments.
- **Efficiency:** Observed **Superlinear Speedup** at 2 and 4 threads due to improved cache utilization.

### 📊 Performance Results
| Threads | Execution Time (s) | Speedup (S) | Efficiency (E) |
| :--- | :--- | :--- | :--- |
| 1 | 38.3517 | 1.00x | 100% |
| 2 | 16.9698 | 2.26x | 112.9% |
| 4 | 9.2761 | 4.13x | 103.3% |
| 8 | 9.4382 | 4.06x | 50.7% |

---

## 🏁 Conclusion
Among the three applications, **Molecular Dynamics** and **Heat Diffusion** demonstrate the best scalability. **Smith–Waterman** is limited by strict data dependencies and frequent synchronization barriers.
