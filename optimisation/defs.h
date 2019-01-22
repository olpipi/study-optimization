#pragma once

#include <Windows.h>
#include <stdint.h>

uint64_t LENGTH = 1000000;
uint64_t threads = 11;

typedef struct {
    uint64_t len;
    double *data;
} my_vector;

/*Optimisations*/
#define ENABLE_OPTIMISATIONS 1


#if ENABLE_OPTIMISATIONS
#ifdef __INTEL_COMPILER
#include "immintrin.h"
#define ALIGNMENT 1
#define VECTORIZATION 1
#else 
#define ALIGNMENT 0
#define VECTORIZATION 0
#endif // __INTEL_COMPILER

#define PARALLELIZATION   1

#else
#define ALIGNMENT 0
#define VECTORIZATION 0
#define PARALLELIZATION   0
#endif // ENABLE_OPTIMISATIONS


#define INTEL_RANDOM_GENERATOR 1


#define START_TIME_MEASURE \
    LARGE_INTEGER TIME_MEASURE_START_TIME, TIME_MEASURE_END_TIME; \
    QueryPerformanceCounter(&TIME_MEASURE_START_TIME);

#define END_TIME_MEASURE(DIFF) \
    QueryPerformanceCounter(&TIME_MEASURE_END_TIME); \
    static LARGE_INTEGER TIME_MEASURE_FREQ = { 0 }; \
    if (!TIME_MEASURE_FREQ.QuadPart) QueryPerformanceFrequency(&TIME_MEASURE_FREQ); \
    DIFF = (double)(TIME_MEASURE_END_TIME.QuadPart - TIME_MEASURE_START_TIME.QuadPart) / (double)TIME_MEASURE_FREQ.QuadPart * 1000000.0;

/*
static inline uint64_t start_clock()
{
    uint32_t cycles_high, cycles_low;
    uint64_t cycles = 0;

    asm volatile ("CPUID\n\t" "RDTSC\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
        "%rax", "%rbx", "%rcx", "%rdx");
    cycles |= cycles_high;
    cycles = cycles << 32;
    cycles |= cycles_low;
    return cycles;
}


static inline uint64_t stop_clock()
{
    uint32_t cycles_high, cycles_low;
    uint64_t cycles = 0;
    asm volatile("RDTSCP\n\t"
        "mov %%edx, %0\n\t" "mov %%eax, %1\n\t"
        "CPUID\n\t": "=r" (cycles_high), "=r" (cycles_low)::
        "%rax", "%rbx", "%rcx", "%rdx");
    cycles |= cycles_high;
    cycles = cycles << 32;
    cycles |= cycles_low;
    return cycles;
}
*/