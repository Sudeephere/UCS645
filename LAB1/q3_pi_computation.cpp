#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    long num_steps = 100000;
    double step = 1.0 / num_steps;
    double sum = 0.0, pi;
    double t1, t2, seq_time;

    // Sequential
    t1 = omp_get_wtime();
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    pi = step * sum;
    t2 = omp_get_wtime();

    seq_time = t2 - t1;
    cout << "Sequential Pi = " << pi << endl;
    cout << "Time = " << seq_time << endl;

    // Parallel
    cout << "\nParallel:\n";
    for (int threads = 2; threads <= 10; threads++) {
        omp_set_num_threads(threads);
        sum = 0.0;

        t1 = omp_get_wtime();
        #pragma omp parallel for reduction(+:sum)
        for (long i = 0; i < num_steps; i++) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
        pi = step * sum;
        t2 = omp_get_wtime();

        double par_time = t2 - t1;
        cout << "Threads=" << threads
             << " Pi=" << pi
             << " Time=" << par_time
             << " Speedup=" << seq_time / par_time << endl;
    }

    return 0;
}
