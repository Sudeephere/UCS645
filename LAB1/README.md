# Parallel Programming Lab (OpenMP)

This repository contains basic OpenMP programs written in **C++** for learning and practicing **shared memory parallel programming**.

---

## 📌 Lab Experiments (Lab 1)

1. **DAXPY Loop (Speedup Analysis)**
   - Parallel loop execution
   - Time comparison: Serial vs Parallel

2. **Matrix Multiplication**
   - 1D Threading approach
   - 2D Threading approach

3. **PI Calculation using Reduction**
   - Uses OpenMP `reduction` clause for fast computation

---

## 🛠️ Requirements
- C++ Compiler (GCC recommended)
- OpenMP Support  
  *(Works on Linux / Windows (MinGW) / Mac with GCC)*

---

## ⚙️ Compilation & Run

### Compile:
```bash
g++ filename.cpp -o output -fopenmp
