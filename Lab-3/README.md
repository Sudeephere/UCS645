# Correlation Matrix Computation - Parallel Programming Assignment

## Overview

This project implements Pearson correlation coefficient computation for matrix row pairs using three different approaches: sequential baseline, OpenMP parallelization, and an optimized version combining multiple performance techniques.

**Goal:** Calculate correlation between every pair of rows in an `ny × nx` matrix and store results in lower triangular format.

**Mathematical Formulation:**

For each pair `(i, j)` where `j ≤ i < ny`:
- Compute: `correlation(row_i, row_j)`
- Store at: `result[i + j * ny]`
- Precision: All operations use **double precision arithmetic**

---

## Implementation Variants

### Version 1: Sequential Baseline
- Single-threaded execution
- Direct correlation computation
- Serves as performance reference point

### Version 2: OpenMP Parallelization
- Multi-threaded using OpenMP directives
- Parallel outer loop with `collapse(2)` optimization
- Dynamic work scheduling

### Version 3: Optimized Implementation
- Pre-normalization strategy (mean subtraction + std dev division)
- OpenMP parallelization with static/dynamic scheduling mix
- SIMD vectorization using `#pragma omp simd`
- Compiler optimizations (-O3 flag)
- Enhanced cache locality

---

## Build & Run

### On WSL (Recommended)

**Navigate to project:**
```bash
cd '/mnt/c/Users/Sudhanshu raj/OneDrive/Documents/TIET subjects/Tiet 6th sem/prallel computing/lab_3'
```

**Compile:**
```bash
make clean && make
```

**Execute:**
```bash
./correlate <ny> <nx>
```

**Examples:**
```bash
./correlate 500 500     # Medium size (recommended)
./correlate 1000 1000   # Large benchmark
```

**With performance statistics:**
```bash
/usr/bin/time -v ./correlate 500 500
```

The program automatically benchmarks all three implementations across multiple thread configurations (1, 2, 4, 8 threads).

**Clean up:**
```bash
make clean
```

### On Windows (MSVC/MinGW)
Compile with OpenMP support enabled and run the executable.

---

## Benchmark Results

### Test Configuration
- **Platform:** WSL2 Ubuntu 22.04
- **Compiler:** g++ 11.4.0 with -O3 -fopenmp
- **Matrix Dimensions:** 500 × 500 (125,250 correlations)
- **Thread Counts Tested:** 1, 2, 4, 8
- **CPU Cores Available:** 8
- **Total Operations:** 313,125,000 floating-point ops

### Performance Summary Table (500×500)

| Threads | Sequential (s) | OpenMP (s) | Fast (s)   | Speedup (Fast) | GFLOPS (Fast) |
|---------|---------------|------------|------------|----------------|---------------|
| 1       | 0.069767      | 0.071047   | 0.069939   | 1.00×          | 4.48          |
| 2       | 0.065185      | 0.034197   | 0.034809   | 2.01×          | 9.00          |
| 4       | 0.065017      | 0.018453   | 0.018726   | 3.73×          | 16.72         |
| 8       | 0.084196      | 0.034772   | 0.027605   | 2.53×          | 11.34         |

---

## Detailed Performance Analysis

### Execution Times by Thread Count (WSL Results)

**Single Thread (Baseline):**
- Sequential: 0.0698 sec (4.49 GFLOPS)
- OpenMP: 0.0710 sec (4.41 GFLOPS)
- Fast: 0.0699 sec (4.48 GFLOPS) ✓ Best single-thread

**2 Threads:**
- Sequential: 0.0652 sec (4.80 GFLOPS)
- OpenMP: 0.0342 sec (9.16 GFLOPS) ✓ Near-linear scaling
- Fast: 0.0348 sec (9.00 GFLOPS)

**4 Threads:**
- Sequential: 0.0650 sec (4.82 GFLOPS)
- OpenMP: 0.0185 sec (16.97 GFLOPS) ✓ Best overall performance
- Fast: 0.0187 sec (16.72 GFLOPS)

**8 Threads:**
- Sequential: 0.0842 sec (3.72 GFLOPS)
- OpenMP: 0.0348 sec (9.01 GFLOPS)
- Fast: 0.0276 sec (11.34 GFLOPS) ✓ Best at 8 threads

### Speedup Metrics (Fast Version)

Using formula: `Speedup = T_baseline_1_thread / T_fast_n_threads`

```
Speedup(1) = 0.069939 / 0.069939 = 1.00× (100.0% efficiency)
Speedup(2) = 0.069939 / 0.034809 = 2.01× (100.5% efficiency)
Speedup(4) = 0.069939 / 0.018726 = 3.73× (93.4% efficiency)
Speedup(8) = 0.069939 / 0.027605 = 2.53× (31.7% efficiency)
```

### Parallel Efficiency

**8-Thread Efficiency Analysis:**
```
Efficiency = Speedup / Number_of_Threads
           = 2.53 / 8
           = 0.317 (31.7%)
```

**Key Insights:**
- Excellent scaling up to 4 threads (93.4% efficiency)
- Significant diminishing returns at 8 threads (31.7% efficiency)
- OpenMP version achieves best peak performance (16.97 GFLOPS @ 4 threads)
- Fast version shows best 8-thread performance (11.34 GFLOPS)
- Performance degradation at 8 threads suggests memory bandwidth bottleneck

---

## Optimization Strategies Employed

### Algorithmic Optimizations
1. **Row Normalization:** Pre-compute normalized values (mean=0, variance=1) to simplify correlation to dot product
2. **Triangular Computation:** Only compute `j ≤ i` since correlation matrix is symmetric
3. **Double Precision:** Maintain accuracy throughout computation

### Parallelization Techniques
4. **OpenMP Threading:** Distribute work across available CPU cores
5. **Load Balancing:** Dynamic scheduling compensates for varying iteration counts
6. **Loop Collapsing:** Flatten nested loops for better thread distribution

### Low-Level Optimizations
7. **SIMD Vectorization:** Compiler auto-vectorization hints for inner loops
8. **Compiler Flags:** `-O3` enables aggressive optimizations
9. **Memory Access Patterns:** Row-major access improves cache hit rates

---

## Key Findings

### Scalability Metrics (WSL Results - 500×500 Matrix)

| Threads | Speedup (Fast) | Efficiency | Performance (GFLOPS) | Best Implementation |
|---------|----------------|------------|----------------------|---------------------|
| 1       | 1.00×          | 100.0%     | 4.48                 | Fast                |
| 2       | 2.01×          | 100.5%     | 9.00                 | OpenMP              |
| 4       | 3.73×          | 93.4%      | 16.97                | OpenMP ⭐ Best Overall |
| 8       | 2.53×          | 31.7%      | 11.34                | Fast                |

**Key Observations:**
- Near-linear speedup up to 4 threads (ideal: 4.0×, actual: 3.73×)
- Performance degradation at 8 threads (speedup drops to 2.53×)
- **Optimal thread count: 4 threads** for this workload

---

### Performance Characteristics by Implementation

| Implementation | 1 Thread (s) | 2 Threads (s) | 4 Threads (s) | 8 Threads (s) | Speedup Range |
|----------------|--------------|---------------|---------------|---------------|---------------|
| Sequential     | 0.0698       | 0.0652        | 0.0650        | 0.0842        | N/A           |
| OpenMP         | 0.0710       | 0.0342        | 0.0185 ⭐     | 0.0348        | 2.08× → 3.85× |
| Fast           | 0.0699       | 0.0348        | 0.0187        | 0.0276        | 2.01× → 2.53× |

**Throughput Metrics:**

| Metric                         | Value                              |
|--------------------------------|------------------------------------|
| Peak throughput                | 6.79 M correlations/sec (OpenMP@4) |
| Fast version @ 8 threads       | 4.54 M correlations/sec            |
| Memory efficiency              | 7.5 MB for 500×500 processing      |
| Best single-thread performance | 4.48 GFLOPS (Fast version)         |

---

### System Resource Usage

| Resource Metric          | Value              | Interpretation                         |
|--------------------------|-------------------|----------------------------------------|
| CPU Utilization          | ~199%             | Effective use of 2 cores avg           |
| Context Switches         | 55 total          | Very low - good thread management      |
| Memory Footprint         | 7,672 KB (7.5 MB) | Efficient cache usage                  |
| Cache Behavior           | Optimized         | Row-major access improves hit rates    |
| Thread Management        | OpenMP dynamic    | Automatic load balancing               |

---

### Computational Complexity Analysis

| Aspect                  | Formula/Value                          | For 500×500 Matrix        |
|-------------------------|----------------------------------------|---------------------------|
| Time Complexity         | O(ny² × nx)                            | 125,250 row pairs         |
| Memory Requirement      | O(ny × nx)                             | Data + normalized arrays  |
| Total Correlations      | ny × (ny + 1) / 2                      | 125,250 correlations      |
| Total Operations        | ~5 ops per element per correlation     | 313M floating-point ops   |
| Best Throughput         | OpenMP @ 4 threads                     | 16.97 GFLOPS              |

---

## Project Structure

```
lab_3/
├── correlate.h                # Function declarations and OpenMP headers
├── correlate.cpp              # Three implementation variants
├── main.cpp                   # Driver with automated benchmarking
├── Makefile                   # Build configuration (g++ -O3 -fopenmp)
├── README.md                  # Documentation (this file)
```

---

## Technical Notes

- **Thread Control:** OpenMP thread count set programmatically via `omp_set_num_threads()`
- **Randomization:** Data filled with uniform random values in [0, 1)
- **Timing:** High-resolution clock used for microsecond precision
- **Compiler:** g++ 11.4.0 with C++11 and OpenMP support
- **Platform:** Tested on WSL2 Ubuntu 22.04 with 8 CPU cores
- **Profiling:** Built with -pg flag for gprof profiling support

---

## Performance Metrics Glossary

**GFLOPS (Giga Floating-Point Operations Per Second):**
- Measures billions of floating-point operations per second
- Higher is better
- Our best: 16.97 GFLOPS (OpenMP @ 4 threads)

**Throughput:**
- Correlations computed per second
- Our best: 6.79 million correlations/sec (OpenMP @ 4 threads)

**Parallel Efficiency:**
- Speedup divided by number of threads
- 100% = perfect linear scaling
- Our results: 32-100% depending on thread count
- Sweet spot: 4 threads with 93.4% efficiency

---

## Additional Resources

- **[PERF_8THREADS_REPORT.md](PERF_8THREADS_REPORT.md)** - In-depth 8-thread performance analysis with system metrics
- **[WSL_USAGE_GUIDE.md](WSL_USAGE_GUIDE.md)** - Complete guide for running on WSL with profiling commands
- **analysis.txt** - Detailed profiling output from test runs

---

## Quick Start (WSL)

```bash
# Navigate to project
cd '/mnt/c/Users/Sudhanshu raj/OneDrive/Documents/TIET subjects/Tiet 6th sem/prallel computing/lab_3'

# Build
make clean && make

# Run benchmark
./correlate 500 500

# Run with detailed stats
/usr/bin/time -v ./correlate 500 500

# Use performance script
chmod +x perf_8threads.sh
./perf_8threads.sh 500
```
