#include <bits/stdc++.h>
#include "bo_base.h"

using namespace std;

Position::Position(int x, int y, int z) : x{ x }, y{ y }, z{ z } {}
Position::Position(tuple<int, int, int> position) {
    auto [xc, yc, zc] = position;
    x = xc;
    y = yc;
    z = zc;
}
bool Position::operator==(const Position& other) const {
    return x == other.x && y == other.y && z == other.z;
}

bool does_fit(const unordered_map<Position, bool> &free_positions, const Package &act_pkg, const Vehicle &veh1, const Position &pos1){
    for (int x = pos1.x; x <= pos1.x + act_pkg.size.x; x++ ){
        for (int y = pos1.y; y <= pos1.y + act_pkg.size.y; y++ ){
            for (int z = pos1.z; z <= pos1.z + act_pkg.size.z; z++ ){
                if(!free_positions.at(Position{x, y, z}) || (pos1.z > 0 && free_positions.at({x, y, z - 1}))){
                    return false;
                }
            }
        }
    }
    return true;
}

vector<Solution> vis_to_solution(const vector<bool> &vis, const vector<pair<Position, int>> &where, const vector<int> &pkg_IDs, const vector<int> &veh_IDs){
    vector<Solution> result;
    for (int i = 0; i < vis.size(); i++){
        if(vis[i]){
            result.emplace_back(where[i].first, pkg_IDs[i], veh_IDs[where[i].second]);
        }
    }
    return result;
}

void fill_map(unordered_map<Position, bool> &positions, Position vec1, Position vec2){
    for (int x = vec1.x; x <= vec2.x; x++ ){
        for (int y = vec1.y; y <= vec2.y; y++ ){
            for (int z = vec1.z; z <= vec2.z; z++ ){
                positions[{x, y, z}] = true;
            }
        }
    }
}

pair<long long, vector<Solution> > knapsack_problem(const vector<Package> &packages, vector<pair<Position, int>> &where_placed, const vector<int> &pkg_IDs,
                                                    vector<unordered_map< Position,  bool>> &free_positions, int ID, long long val, vector<long long> &weights,
                                                    const vector<long long> &prefix_sum, vector<bool> &taken, const vector<Vehicle> &vehicles, const vector<int> &veh_IDs)
                                                    {
    if (ID == packages.size()) {
        return {val, vis_to_solution(taken, where_placed, pkg_IDs, veh_IDs)};
    }

    Package act_pkg = packages[ID];
    pair<long long, vector<Solution>> dont_take_res;
    pair<long long, vector<Solution>> maxi_val;

    dont_take_res = knapsack_problem(packages, where_placed, pkg_IDs, free_positions, ID + 1, val, weights, prefix_sum,
                                     taken, vehicles, veh_IDs);
    long long dont_take = dont_take_res.first;
    long long max_res = 0;
    taken[ID] = true;
    for (int i = 0; i < vehicles.size(); i++) {
        Vehicle veh1 = vehicles[i];
        for (auto const &[pos1, free]: free_positions[i]) {
            if (free && pos1.x + act_pkg.size.x <= veh1.size.x && pos1.y + act_pkg.size.y <= veh1.size.y &&
                pos1.z + act_pkg.size.z <= veh1.size.z) {
                if (weights[i] + act_pkg.weight <= veh1.weight_limit && does_fit(free_positions[i], act_pkg, veh1, pos1)) {
                    for (int x = pos1.x; x <= pos1.x + act_pkg.size.x; x++) {
                        for (int y = pos1.y; y <= pos1.y + act_pkg.size.y; y++) {
                            for (int z = pos1.z; z <= pos1.z + act_pkg.size.z; z++) {
                                free_positions[i][Position{x, y, z}] = false;
                            }
                        }
                    }

                    where_placed[ID] = pair(Position(pos1.x, pos1.y, pos1.z), i);
                    weights[i] += act_pkg.weight;
                    pair<long long, vector<Solution>> take_res = knapsack_problem(packages, where_placed, pkg_IDs,
                                                                                  free_positions, ID + 1, val + act_pkg.cost,
                                                                                  weights, prefix_sum,
                                                                                  taken, vehicles, veh_IDs);
                    if (take_res.first > max_res) {
                        maxi_val = std::move(take_res);
                        max_res = maxi_val.first;
                    }
                    weights[i] -= act_pkg.weight;
                    for (int x = pos1.x; x <= pos1.x + act_pkg.size.x; x++) {
                        for (int y = pos1.y; y <= pos1.y + act_pkg.size.y; y++) {
                            for (int z = pos1.z; z <= pos1.z + act_pkg.size.z; z++) {
                                free_positions[i][Position{x, y, z}] = true;
                            }
                        }
                    }
                    if (ID < packages.size() - 1 && max_res - val == prefix_sum[ID + 1] && max_res >= dont_take) {
                        taken[ID] = false;
                        return maxi_val;
                    }
                }
            }
        }
    }
    taken[ID] = false;
    if (max_res >= dont_take) {
        return maxi_val;
    }
    return dont_take_res;
}


ostream& operator<<(ostream& os, const Position &pos) {
    os << '(' << pos.x << ", " << pos.y << ", " << pos.z << ')';
    return os;
}

Package::Package(Position size, int weight, int cost)
        : size{ size }, weight{ weight }, cost{ cost } {
}


Vehicle::Vehicle(Position size, int weight_limit)
        : size{ size }, weight_limit{ weight_limit } {
}

Solution::Solution(Position position, int package_id, int vehicle_id)
        : package_position{ position }, package_id{ package_id },
          vehicle_id{ vehicle_id } {
}

ostream& operator<<(ostream& os, const Solution& sol) {
    os << "package_id: " << sol.package_id << " vehicle_id: " << sol.vehicle_id
       << " package_position: " << sol.package_position << " ";
    return os;
}

bool check_solution_feasibility(const vector<Solution>& solutions,
                                const vector<Package>& packages,
                                const vector<Vehicle>& vehicles) {
    unordered_map<int, vector<Solution>> vehicles_to_solutions;
    unordered_map<int, long long> vehicles_to_weight;
    for (auto sol : solutions) {
        vehicles_to_weight[sol.vehicle_id] += packages[sol.package_id].weight;
        if (vehicles_to_weight[sol.vehicle_id] >
            vehicles[sol.vehicle_id].weight_limit) {
            return false;
        }

        if (sol.package_position.x + packages[sol.package_id].size.x >
            vehicles[sol.vehicle_id].size.x ||
            sol.package_position.y + packages[sol.package_id].size.y >
            vehicles[sol.vehicle_id].size.y ||
            sol.package_position.z + packages[sol.package_id].size.z >
            vehicles[sol.vehicle_id].size.z) {
            return false;
        }

        vehicles_to_solutions[sol.vehicle_id].push_back(sol);
    }
    for (auto& [_, solutions_per_vech] : vehicles_to_solutions) {
        sort(solutions_per_vech.begin(), solutions_per_vech.end(),
             [](Solution a, Solution b) {
                 return (a.package_position.z < b.package_position.z);
             });
    }
    for (auto& [vehicle_id, solutions_per_vech] : vehicles_to_solutions) {
        unordered_set<Position> filled_positions;
        for (auto sol : solutions_per_vech) {
            for (int i = 0; i < packages[sol.package_id].size.x; ++i) {
                for (int j = 0; j < packages[sol.package_id].size.y; ++j) {
                    if (sol.package_position.z > 0 &&
                        filled_positions.count({ sol.package_position.x + i,
                                                 sol.package_position.y + j,
                                                 sol.package_position.z - 1 }) == 0) {
                        return false;
                    }
                    for (int k = 0; k < packages[sol.package_id].size.z; ++k) {
                        auto [_, inserted] = filled_positions.insert(
                                { sol.package_position.x + i, sol.package_position.y + j,
                                  sol.package_position.z + k });
                        if (!inserted) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

tuple<vector<Solution>, int> compute_solutions(vector<Package>& packages,
                                               vector<Vehicle>& vehicles) {
    int costs_sum = 0;
    vector<Solution> solutions;
    unordered_map<int, long long> vehicle_weights;
    unordered_map<int, unordered_set<Position>> vehicle_filled_positions;

    sort(packages.begin(), packages.end(),
         [](const Package& a,
            const Package& b) {
             return b.cost <= a.cost;
         });

    for (size_t package_id = 0; package_id < packages.size(); ++package_id) {
        const Package& package = packages[package_id];

        for (size_t vehicle_id = 0; vehicle_id < vehicles.size(); ++vehicle_id) {
            Vehicle& vehicle = vehicles[vehicle_id];

            if (vehicle_weights[vehicle_id] + package.weight > vehicle.weight_limit) {
                continue; // cannot fit because of weight
            }

            bool placed = false;
            for (int x = vehicle.size.x - package.size.x; x >= 0 && !placed; --x) {
                for (int y = vehicle.size.y - package.size.y; y >= 0 && !placed; --y) {
                    for (int z = 0; z + package.size.z <= vehicle.size.z && !placed;
                         ++z) {
                        Position pos(x, y, z);

                        bool valid = true;
                        for (int dx = 0; dx < package.size.x && valid; ++dx) {
                            for (int dy = 0; dy < package.size.y && valid; ++dy) {
                                for (int dz = 0; dz < package.size.z && valid; ++dz) {
                                    Position new_pos(x + dx, y + dy, z + dz);
                                    if (vehicle_filled_positions[vehicle_id].count(new_pos)) {
                                        valid = false;
                                    }
                                }
                            }
                        }

                        if (!valid)
                            continue;

                        if (z > 0) {
                            bool supported = true;
                            for (int dx = 0; dx < package.size.x && supported; ++dx) {
                                for (int dy = 0; dy < package.size.y && supported; ++dy) {
                                    Position below_pos(x + dx, y + dy, z - 1);
                                    if (vehicle_filled_positions[vehicle_id].count(below_pos) ==
                                        0) {
                                        supported = false;
                                    }
                                }
                            }
                            if (!supported)
                                continue;
                        }

                        solutions.emplace_back(pos, package_id, vehicle_id);
                        vehicle_weights[vehicle_id] += package.weight;

                        for (int dx = 0; dx < package.size.x; ++dx) {
                            for (int dy = 0; dy < package.size.y; ++dy) {
                                for (int dz = 0; dz < package.size.z; ++dz) {
                                    vehicle_filled_positions[vehicle_id].insert(
                                            Position(x + dx, y + dy, z + dz));
                                }
                            }
                        }
                        placed = true;
                        costs_sum += package.cost;
                    }
                }
            }
            if (placed)
                break;
        }
    }

    return { solutions, costs_sum };
}