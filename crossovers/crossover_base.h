// crossover/crossover_base.h
#pragma once
#include <vector>

using Assignment = std::vector<int>;

class CrossoverStrategy {
public:
    virtual Assignment crossover(const Assignment& parent1, const Assignment& parent2) = 0;
    virtual ~CrossoverStrategy() = default;
};