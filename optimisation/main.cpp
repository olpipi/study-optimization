// optimisation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <omp.h>

#include "Algorithm.h"
#include "Algorithm_optimized.h"


int main()
{
    uint64_t len_list[] = { 1000, 10000, 100000, 1000000 };


    for (auto len : len_list) {
        std::cout << "len=" << len << "\n";
        LENGTH = len;
        uint64_t num = 1000000000 / LENGTH;
        uint64_t i;
        //heating
        for (i = 0; i < num / 100; i++) {
            main_algo_optimized();
            main_algo();
        }

        double res = 0;
        
        for (i = 0; i < num; i++)
            res += main_algo();
        std::cout << "not optimazed algo = " << res / num << "\n";

        res = 0;

        for (i = 0; i < num; i++)
            res += main_algo_optimized();
        std::cout << "optimized algo     = " << res / num << "\n";
        //std::cout << res / num << "\n";
    }
}

