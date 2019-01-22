#pragma once


#include <cstdlib>
#include "defs.h"

#include "IntelGenerator.h"


long my_vector_len(my_vector *v) {
    return v->len;
}

void foo_bar(my_vector *v, double *res)
{
    long i;
    *res = 1;
    for (i = 0; i < my_vector_len(v); i++) {
        *res *= v->data[i];
    }
}


double main_algo() {
    double *a;
    my_vector v;

    a = (double*)malloc(sizeof(double) * LENGTH);

    v.len = LENGTH;
    v.data = a;


    double res;
#if INTEL_RANDOM_GENERATOR
    getRandomArray(0.01, 2, LENGTH, a);
#else
    for (int i = 0; i < LENGTH; i++)
        a[i] = (double)rand() / (double)RAND_MAX + 1.0;
#endif
    double diff;


    START_TIME_MEASURE

    foo_bar(&v, &res);

    END_TIME_MEASURE(diff)

    free(a);
    return diff;
}
