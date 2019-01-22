#pragma once

#include "mkl.h"
#include <Windows.h>


WORD getStandartSeed() {
    SYSTEMTIME* st = new SYSTEMTIME();
    GetSystemTime(st);
    return st->wMilliseconds;
}

VSLStreamStatePtr InitGen() {
    VSLStreamStatePtr stream;
    WORD BIG_SEED = getStandartSeed();
    const unsigned int seed[2] = { BIG_SEED, BIG_SEED };
    vslNewStreamEx(&stream, VSL_BRNG_MCG59, 2, seed);
    return stream;
}

void FreeGen(VSLStreamStatePtr stream) {
    vslDeleteStream(&stream);
}

void GenerateGauss(double a, double b, int count,
    VSLStreamStatePtr stream, double *r)
{
    vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, count, r, a, b);
}

void getRandomArray(double leftBound, double rightBound, int size, double * buffer) {
    VSLStreamStatePtr stream = InitGen();
    GenerateGauss(leftBound, rightBound, size, stream, buffer);
    FreeGen(stream);
}
