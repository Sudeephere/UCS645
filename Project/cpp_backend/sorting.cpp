#include "sorting.h"
#include <omp.h>
#include <iostream>

// Helper merge function
void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Serial Merge Sort Implementation
void serialMergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        serialMergeSort(arr, left, mid);
        serialMergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

// Internal recursive function for parallel merge sort using OpenMP tasks
void parallelMergeSortImpl(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        // Use a tiny safety threshold (100) to prevent exploding task creation
        // which can lead to huge overhead and crashes. 
        if (right - left + 1 <= 100) {
            serialMergeSort(arr, left, right);
        } else {
            int mid = left + (right - left) / 2;

            #pragma omp task shared(arr)
            parallelMergeSortImpl(arr, left, mid);
            
            #pragma omp task shared(arr)
            parallelMergeSortImpl(arr, mid + 1, right);

            #pragma omp taskwait
            merge(arr, left, mid, right);
        }
    }
}

// Public wrapper for parallel merge sort
void parallelMergeSort(std::vector<int>& arr, int left, int right) {
    #pragma omp parallel
    {
        #pragma omp single
        parallelMergeSortImpl(arr, left, right);
    }
}

// Internal recursive function for adaptive merge sort
void adaptiveMergeSortImpl(std::vector<int>& arr, int left, int right, int threshold) {
    if (left < right) {
        // Dynamically choose sorting strategy based on array size
        if (right - left + 1 <= threshold) {
            serialMergeSort(arr, left, right);
        } else {
            int mid = left + (right - left) / 2;

            #pragma omp task shared(arr)
            adaptiveMergeSortImpl(arr, left, mid, threshold);
            
            #pragma omp task shared(arr)
            adaptiveMergeSortImpl(arr, mid + 1, right, threshold);

            #pragma omp taskwait
            merge(arr, left, mid, right);
        }
    }
}

// Public wrapper for adaptive merge sort
void adaptiveMergeSort(std::vector<int>& arr, int left, int right, int threshold) {
    #pragma omp parallel
    {
        #pragma omp single
        adaptiveMergeSortImpl(arr, left, right, threshold);
    }
}

// Verification function
bool isSorted(const std::vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < arr[i - 1]) {
            return false;
        }
    }
    return true;
}
