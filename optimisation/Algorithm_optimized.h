#pragma once

#include <cstdlib>
#include "defs.h"
#include <malloc.h>
#include <iostream>
#include <chrono>


#include "IntelGenerator.h"

uint64_t my_vector_len_optimized(my_vector *v) {
    return v->len;
}

inline void foo_bar_optimized(my_vector *v, double *res)
{
    uint64_t len = my_vector_len_optimized(v);
    double *data = v->data;
    double _res = 1;

#if VECTORIZATION
#if PARALLELIZATION
    omp_set_num_threads(threads);
#pragma omp parallel firstprivate(data, len) reduction(*:_res)
    {
        double res4[4] = { 1, 1, 1, 1 };
        __m256d res256 = _mm256_loadu_pd(res4);
        __m256d tmp256;
        len /= 4;
#pragma omp for
        for (uint64_t i = 0; i < len; i++) {
            tmp256 = _mm256_loadu_pd(data + i * 4);
            res256 = _mm256_mul_pd(res256, tmp256);
        }
        _mm256_store_pd(res4, res256);

        _res = res4[0] * res4[1] * res4[2] * res4[3];
    }

#else //PARALLELIZATION
        double res4[4] = { 1, 1, 1, 1 };
        __m256d res256 = _mm256_loadu_pd(res4);
        for (uint64_t i = 0; i < len; i += 4) {
            __m256d tmp256 = _mm256_loadu_pd(data + i);
            res256 = _mm256_mul_pd(res256, tmp256);
        }
        _mm256_store_pd(res4, res256);

        _res = res4[0] * res4[1] * res4[2] * res4[3];


#endif //PARALLELIZATION
#else //VECTORIZATION
    *res = 1;
    data ++;
#if PARALLELIZATION
    omp_set_num_threads(threads);

#pragma omp parallel reduction(*:_res)
    {
#pragma omp for
        for (int i = 0; i < len; i++) {
            _res *= data[i];
        }
    }
#else
    for (uint64_t i = 0; i < len; i++) {
        _res *= *(data++);
    }
#endif
#endif


    *res = _res;

}


double main_algo_optimized() {

    double *a_optimized;
    my_vector v_optimized;

    double res;

#if ALIGNMENT
    a_optimized = (double*)_mm_malloc(sizeof(double) * LENGTH, 64);
#else
    a_optimized = (double*)malloc(sizeof(double) * LENGTH);
#endif

    v_optimized.len = LENGTH;
    v_optimized.data = a_optimized;
#if INTEL_RANDOM_GENERATOR
    getRandomArray(0.01, 2, LENGTH, a_optimized);
#else
    for (int i = 0; i < LENGTH; i++)
        a_optimized[i] = (double)rand() / (double)RAND_MAX + 1.0;
#endif

    int i = 0;

    double diff;
    START_TIME_MEASURE

    foo_bar_optimized(&v_optimized, &res);

    END_TIME_MEASURE(diff)

#if ALIGNMENT
    _mm_free(a_optimized);
#else
    free(a_optimized);
#endif
    return diff;
}