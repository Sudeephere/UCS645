#include <iostream>
#include <omp.h>
using namespace std;

double A[1000][1000], B[1000][1000], C[1000][1000];

int main() {
    int N = 1000;
    double t1, t2, seq_time;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            A[i][j] = 1.0;
            B[i][j] = 2.0;
            C[i][j] = 0.0;
        }

    // Sequential
    t1 = omp_get_wtime();
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                C[i][j] += A[i][k] * B[k][j];
    t2 = omp_get_wtime();

    seq_time = t2 - t1;
    cout << "Sequential Time = " << seq_time << endl;

    // 1D Parallel
    cout << "\n1D Parallel:\n";
    for (int threads = 2; threads <= 10; threads++) {
        omp_set_num_threads(threads);

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                C[i][j] = 0.0;

        t1 = omp_get_wtime();
        #pragma omp parallel for private(j, k)
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                for (int k = 0; k < N; k++)
                    C[i][j] += A[i][k] * B[k][j];
        t2 = omp_get_wtime();

        double par_time = t2 - t1;
        cout << "Threads=" << threads
             << " Time=" << par_time
             << " Speedup=" << seq_time / par_time << endl;
    }

    // 2D Parallel
    cout << "\n2D Parallel:\n";
    for (int threads = 2; threads <= 10; threads++) {
        omp_set_num_threads(threads);

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                C[i][j] = 0.0;

        t1 = omp_get_wtime();
        #pragma omp parallel for collapse(2) private(k)
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                for (int k = 0; k < N; k++)
                    C[i][j] += A[i][k] * B[k][j];
        t2 = omp_get_wtime();

        double par_time = t2 - t1;
        cout << "Threads=" << threads
             << " Time=" << par_time
             << " Speedup=" << seq_time / par_time << endl;
    }

    return 0;
}
