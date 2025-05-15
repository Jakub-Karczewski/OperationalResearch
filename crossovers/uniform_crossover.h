#pragma once
#include "crossover_base.h"
#include <cstdlib>

class UniformCrossover : public CrossoverStrategy {
public:
    Assignment crossover(const Assignment& p1, const Assignment& p2) override {
        Assignment child(p1.size());
        for (size_t i = 0; i < p1.size(); i++) {
            child[i] = (rand() % 2 == 0) ? p1[i] : p2[i];
        }
        return child;
    }
};