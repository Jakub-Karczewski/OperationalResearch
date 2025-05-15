#pragma once
#include "crossover_base.h"
#include <cstdlib>

class OnePointCrossover : public CrossoverStrategy {
public:
    Assignment crossover(const Assignment& p1, const Assignment& p2) override {
        size_t size = p1.size();
        size_t point = rand() % size;

        Assignment child(size);
        for (size_t i = 0; i < size; ++i) {
            if (i <= point)
                child[i] = p1[i];
            else
                child[i] = p2[i];
        }
        return child;
    }
};