#pragma once
#include "crossover_base.h"
#include "../bo_base.h"
#include <vector>
#include <cstdlib>
#include <unordered_map>

class HeuristicCrossover : public CrossoverStrategy {
private:
    const std::vector<Package>& packages;
    const std::vector<Vehicle>& vehicles;

public:
    HeuristicCrossover(const std::vector<Package>& pkgs, const std::vector<Vehicle>& vehs)
        : packages(pkgs), vehicles(vehs) {}

    Assignment crossover(const Assignment& p1, const Assignment& p2) override {
        Assignment child(p1.size());
        std::unordered_map<int, int> vehicle_load;

        for (size_t i = 0; i < p1.size(); i++) {
            int veh1 = p1[i];
            int veh2 = p2[i];

            int load1 = vehicle_load[veh1];
            int load2 = vehicle_load[veh2];
            if (load1 < load2) {
                child[i] = veh1;
            } else if (load2 < load1) {
                child[i] = veh2;
            } else {
                child[i] = (rand() % 2 == 0) ? veh1 : veh2;
            }
            vehicle_load[child[i]]++;
        }
        return child;
    }
};