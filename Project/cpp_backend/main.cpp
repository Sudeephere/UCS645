#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <omp.h>
#include <iomanip>
#include <string>
#include "sorting.h"

// Generate random array
std::vector<int> generateRandomArray(int size) {
    std::vector<int> arr(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);

    for (int i = 0; i < size; ++i) {
        arr[i] = dis(gen);
    }
    return arr;
}

// Function to measure execution time
template<typename Func>
double measureTime(Func f) {
    auto start = std::chrono::high_resolution_clock::now();
    f();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

int main(int argc, char* argv[]) {
    std::vector<int> sizes;
    bool json_output = false;

    if (argc > 1) {
        int size = std::stoi(argv[1]);
        if (size > 0) {
            sizes.push_back(size);
            json_output = true;
        }
    } else {
        sizes = {10000, 100000, 1000000, 5000000};
    }

    int threshold = 10000; // Switch to serial when chunk size <= 10,000
    int max_threads = omp_get_max_threads();

    if (!json_output) {
        std::cout << "Adaptive Parallel Sorting System" << std::endl;
        std::cout << "================================" << std::endl;
        std::cout << "Max OpenMP Threads Available: " << max_threads << std::endl;
        std::cout << "Threshold for Adaptive Sort: " << threshold << " elements" << std::endl;
        std::cout << "----------------------------------------------------------------------\n";
        std::cout << std::left << std::setw(15) << "Array Size" 
                  << std::setw(15) << "Serial (ms)" 
                  << std::setw(20) << "Naive Parallel (ms)" 
                  << std::setw(15) << "Adaptive (ms)" << std::endl;
        std::cout << "----------------------------------------------------------------------\n";
    }

    for (int size : sizes) {
        std::vector<int> original = generateRandomArray(size);

        std::vector<int> arr1 = original;
        std::vector<int> arr2 = original;
        std::vector<int> arr3 = original;

        // Serial Sort
        double time_serial = measureTime([&]() {
            serialMergeSort(arr1, 0, arr1.size() - 1);
        });
        if (!isSorted(arr1)) std::cerr << "Error: Serial sort failed to sort correctly!" << std::endl;

        // Naive Parallel Sort
        double time_parallel = measureTime([&]() {
            parallelMergeSort(arr2, 0, arr2.size() - 1);
        });
        if (!isSorted(arr2)) std::cerr << "Error: Parallel sort failed to sort correctly!" << std::endl;

        // Adaptive Sort
        double time_adaptive = measureTime([&]() {
            adaptiveMergeSort(arr3, 0, arr3.size() - 1, threshold);
        });
        if (!isSorted(arr3)) std::cerr << "Error: Adaptive sort failed to sort correctly!" << std::endl;

        if (json_output) {
            std::cout << "{"
                      << "\"size\":" << size << ","
                      << "\"serial_ms\":" << time_serial << ","
                      << "\"parallel_ms\":" << time_parallel << ","
                      << "\"adaptive_ms\":" << time_adaptive << ","
                      << "\"threads\":" << max_threads
                      << "}" << std::endl;
        } else {
            std::cout << std::left << std::setw(15) << size 
                      << std::setw(15) << std::fixed << std::setprecision(2) << time_serial 
                      << std::setw(20) << time_parallel 
                      << std::setw(15) << time_adaptive << std::endl;
        }
    }
    
    if (!json_output) {
        std::cout << "----------------------------------------------------------------------\n";
        std::cout << "Execution completed successfully." << std::endl;
    }

    return 0;
}
