#pragma once
#include <bits/stdc++.h>

using namespace std;

struct Position {
    Position(int x, int y, int z);
    Position(tuple<int, int, int> position);
    bool operator==(const Position& other) const;
    int x, y, z;
};

ostream& operator<<(ostream& os, const Position& pos);

namespace std {
    template <> struct hash<Position> {
        size_t operator()(const Position& pos) const noexcept {
            size_t h1 = hash<int>{}(pos.x);
            size_t h2 = hash<int>{}(pos.y);
            size_t h3 = hash<int>{}(pos.z);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
} // namespace std

struct Package {
    Package(Position size, int weight, int cost);
    Position size;
    int weight;
    int cost;
};

struct Vehicle {
    Vehicle(Position size, int weight_limit);
    Position size;
    int weight_limit;
};

struct Solution {
    Solution(Position position, int package_id, int vehicle_id);
    int package_id;
    int vehicle_id;
    Position package_position;
};

ostream& operator<<(ostream& os, const Solution& sol);

bool check_solution_feasibility(const vector<Solution>& solutions,
                                const vector<Package>& packages,
                                const vector<Vehicle>& vehicles);

void fill_map(unordered_map<Position, bool> &positions, Position vec1, Position vec2);

vector<Solution> vis_to_solution(const vector<bool> &vis, const vector<pair<Position, int>> &where, const vector<int> &pkg_IDs, const vector<int> &veh_IDs);

bool does_fit(const unordered_map<Position, bool> &free_positions, const Package &act_pkg, const Vehicle &veh1, const Position &pos1);



tuple<vector<Solution>, int> compute_solutions(vector<Package>& packages,
                                               vector<Vehicle>& vehicles);

pair<long long, vector<Solution> > knapsack_problem(const vector<Package> &packages, vector<pair<Position, int>> &where_placed, const vector<int> &pkg_IDs,
                                                    vector<unordered_map< Position,  bool>> &free_positions, int ID, long long val, vector<long long> &weights,
                                                    const vector<long long> &prefix_sum, vector<bool> &taken, const vector<Vehicle> &vehicles, const vector<int> &veh_IDs);