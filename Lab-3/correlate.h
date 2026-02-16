#ifndef CORRELATE_H
#define CORRELATE_H

#ifdef _OPENMP
#include <omp.h>
#endif

/**
 * Calculate correlation coefficients between all pairs of rows
 *
 * @param ny Number of rows (vectors)
 * @param nx Number of columns (elements per vector)
 * @param data Input matrix stored in row-major order
 *             Element at row y, column x is at data[x + y*nx]
 * @param result Output correlation matrix (lower triangular)
 *               For all 0 <= j <= i < ny, the Pearson correlation
 *               coefficient between row i and row j is stored
 *               at result[i + j*ny]
 */
void correlate(int ny, int nx, const float* data, float* result);

#endif // CORRELATE_H
