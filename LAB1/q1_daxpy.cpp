#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int N = 65536;
    double X[N], Y[N];
    double a = 2.5;
    double t1, t2, seq_time;

    for (int i = 0; i < N; i++) {
        X[i] = 1.0;
        Y[i] = 2.0;
    }

    // Sequential
    t1 = omp_get_wtime();
    for (int i = 0; i < N; i++) {
        X[i] = a * X[i] + Y[i];
    }
    t2 = omp_get_wtime();
    seq_time = t2 - t1;

    cout << "Sequential Time = " << seq_time << " seconds\n";

    // Parallel
    for (int threads = 2; threads <= 12; threads++) {
        omp_set_num_threads(threads);

        for (int i = 0; i < N; i++)
            X[i] = 1.0;

        t1 = omp_get_wtime();
        #pragma omp parallel for
        for (int i = 0; i < N; i++) {
            X[i] = a * X[i] + Y[i];
        }
        t2 = omp_get_wtime();

        double par_time = t2 - t1;
        double speedup = seq_time / par_time;

        cout << "Threads = " << threads
             << " Time = " << par_time
             << " Speedup = " << speedup << endl;
    }

    return 0;
}
