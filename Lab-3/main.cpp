#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include <cstring>
#include <omp.h>

void correlate_seq(int ny, int nx, const float* data, float* result);
void correlate_omp(int ny, int nx, const float* data, float* result);
void correlate_fast(int ny, int nx, const float* data, float* result);

// Performance statistics structure
struct PerfStats {
    double time_sec;
    long long total_correlations;
    long long total_operations;
    double throughput;  // correlations per second
    double gflops;      // approximate GFLOPS

    void calculate(int ny, int nx) {
        total_correlations = (long long)ny * (ny + 1) / 2;
        // Each correlation: nx multiplications, nx additions, mean calcs, etc
        // Rough estimate: ~5 ops per element for correlation
        total_operations = total_correlations * nx * 5;
        throughput = total_correlations / time_sec;
        gflops = (total_operations / 1e9) / time_sec;
    }

    void print(const char* version_name, int threads) {
        std::cout << std::fixed << std::setprecision(6);
        std::cout << version_name << " (" << threads << " threads):\n";
        std::cout << "  Time:              " << time_sec << " sec\n";
        std::cout << "  Throughput:        " << std::setprecision(2)
                  << throughput << " correlations/sec\n";
        std::cout << "  Performance:       " << std::setprecision(3)
                  << gflops << " GFLOPS (approx)\n";
        std::cout << "  Total operations:  " << total_operations << "\n";
        std::cout << std::endl;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: ./correlate <ny> <nx>\n";
        return 1;
    }

    int ny = std::atoi(argv[1]);
    int nx = std::atoi(argv[2]);

    std::cout << "========================================\n";
    std::cout << "Correlation Matrix Performance Benchmark\n";
    std::cout << "========================================\n";
    std::cout << "Matrix size: " << ny << " x " << nx << "\n";
    std::cout << "Total correlations to compute: "
              << (long long)ny * (ny + 1) / 2 << "\n";
    std::cout << "Max threads available: " << omp_get_max_threads() << "\n";
    std::cout << "========================================\n";

    float* data = new float[ny * nx];
    float* result = new float[ny * ny];

    // Fill matrix with random values (fixed seed for reproducibility)
    srand(42);
    for (int i = 0; i < ny * nx; i++)
        data[i] = static_cast<float>(rand()) / RAND_MAX;

    int thread_counts[] = {1, 2, 4, 8};

    // Store all results for summary table
    PerfStats seq_stats[4], omp_stats[4], fast_stats[4];

    for (int idx = 0; idx < 4; idx++) {
        int t = thread_counts[idx];
        std::cout << "\n========== " << t << " Thread(s) ==========\n\n";
        omp_set_num_threads(t);

        // Sequential
        auto start = std::chrono::high_resolution_clock::now();
        correlate_seq(ny, nx, data, result);
        auto end = std::chrono::high_resolution_clock::now();
        seq_stats[idx].time_sec = std::chrono::duration<double>(end - start).count();
        seq_stats[idx].calculate(ny, nx);
        seq_stats[idx].print("Sequential", t);

        // OpenMP
        start = std::chrono::high_resolution_clock::now();
        correlate_omp(ny, nx, data, result);
        end = std::chrono::high_resolution_clock::now();
        omp_stats[idx].time_sec = std::chrono::duration<double>(end - start).count();
        omp_stats[idx].calculate(ny, nx);
        omp_stats[idx].print("OpenMP", t);

        // Fast
        start = std::chrono::high_resolution_clock::now();
        correlate_fast(ny, nx, data, result);
        end = std::chrono::high_resolution_clock::now();
        fast_stats[idx].time_sec = std::chrono::duration<double>(end - start).count();
        fast_stats[idx].calculate(ny, nx);
        fast_stats[idx].print("Fast", t);
    }

    // Print summary table
    std::cout << "\n========================================\n";
    std::cout << "PERFORMANCE SUMMARY TABLE\n";
    std::cout << "========================================\n\n";

    std::cout << "Execution Times (seconds):\n";
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Threads | Sequential | OpenMP     | Fast       | Speedup (Fast)\n";
    std::cout << "--------|------------|------------|------------|---------------\n";
    for (int i = 0; i < 4; i++) {
        double speedup = fast_stats[0].time_sec / fast_stats[i].time_sec;
        std::cout << std::setw(7) << thread_counts[i] << " | "
                  << std::setw(10) << seq_stats[i].time_sec << " | "
                  << std::setw(10) << omp_stats[i].time_sec << " | "
                  << std::setw(10) << fast_stats[i].time_sec << " | "
                  << std::setw(13) << std::setprecision(2) << speedup << "x\n";
    }

    std::cout << "\n\nPerformance (GFLOPS):\n";
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Threads | Sequential | OpenMP     | Fast\n";
    std::cout << "--------|------------|------------|----------\n";
    for (int i = 0; i < 4; i++) {
        std::cout << std::setw(7) << thread_counts[i] << " | "
                  << std::setw(10) << seq_stats[i].gflops << " | "
                  << std::setw(10) << omp_stats[i].gflops << " | "
                  << std::setw(8) << fast_stats[i].gflops << "\n";
    }

    std::cout << "\n\nParallel Efficiency (Fast version):\n";
    std::cout << "Threads | Speedup | Efficiency\n";
    std::cout << "--------|---------|------------\n";
    for (int i = 0; i < 4; i++) {
        double speedup = fast_stats[0].time_sec / fast_stats[i].time_sec;
        double efficiency = speedup / thread_counts[i] * 100.0;
        std::cout << std::setw(7) << thread_counts[i] << " | "
                  << std::setw(7) << std::setprecision(2) << speedup << "x | "
                  << std::setw(9) << std::setprecision(1) << efficiency << "%\n";
    }

    std::cout << "\n========================================\n";

    delete[] data;
    delete[] result;

    return 0;
}