#ifndef SORTING_H
#define SORTING_H

#include <vector>

// Serial Merge Sort
void serialMergeSort(std::vector<int>& arr, int left, int right);

// Parallel Merge Sort
void parallelMergeSort(std::vector<int>& arr, int left, int right);

// Adaptive Merge Sort (switches to serial when chunk size <= threshold)
void adaptiveMergeSort(std::vector<int>& arr, int left, int right, int threshold);

// Helper to check if array is sorted
bool isSorted(const std::vector<int>& arr);

#endif
