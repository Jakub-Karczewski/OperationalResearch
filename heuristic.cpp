#include <bits/stdc++.h>
#include "heuristic.h"

using namespace std;

/* chyba tego nie powinno byc, z tym mi sie nie kompiluje
ostream& operator<<(ostream& os, const Solution& sol) {
    os << "package_id: " << sol.package_id << " vehicle_id: " << sol.vehicle_id
       << " package_position: " << sol.package_position << " ";
    return os;
}

bool check_solution_feasibility(const vector<Solution> &solutions,
                                const vector<Package> &packages,
                                const vector<Vehicle> &vehicles) {
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
  for (auto &[_, solutions_per_vech] : vehicles_to_solutions) {
    sort(solutions_per_vech.begin(), solutions_per_vech.end(),
         [](Solution a, Solution b) {
           return (a.package_position.z < b.package_position.z);
         });
  }
  for (auto &[vehicle_id, solutions_per_vech] : vehicles_to_solutions) {
    unordered_set<Position> filled_positions;
    for (auto sol : solutions_per_vech) {
      for (int i = 0; i < packages[sol.package_id].size.x; ++i) {
        for (int j = 0; j < packages[sol.package_id].size.y; ++j) {
          if (sol.package_position.z > 0 &&
              filled_positions.count({sol.package_position.x + i,
                                      sol.package_position.y + j,
                                      sol.package_position.z - 1}) == 0) {
            return false;
          }
          for (int k = 0; k < packages[sol.package_id].size.z; ++k) {
            auto [_, inserted] = filled_positions.insert(
                {sol.package_position.x + i, sol.package_position.y + j,
                 sol.package_position.z + k});
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

tuple<vector<Solution>, int> compute_solutions(vector<Package> &packages,
                                               vector<Vehicle> &vehicles) {
  int costs_sum = 0;
  vector<Solution> solutions;
  unordered_map<int, long long> vehicle_weights;
  unordered_map<int, unordered_set<Position>> vehicle_filled_positions;

  for (size_t package_id = 0; package_id < packages.size(); ++package_id) {
    const Package &package = packages[package_id];

    for (size_t vehicle_id = 0; vehicle_id < vehicles.size(); ++vehicle_id) {
      Vehicle &vehicle = vehicles[vehicle_id];

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

  return {solutions, costs_sum};
} 
*/

int main() {
  int num_of_vehicles, num_of_packages;

  cin >> num_of_vehicles;
  vector<Vehicle> vehicles;

  for (int i = 0; i < num_of_vehicles; i++) {
    int x, y, z, w;
    cin >> x >> y >> z >> w;
    vehicles.emplace_back(Position{x, y, z}, w);
  }

  cin >> num_of_packages;
  vector<Package> packages;

  for (int i = 0; i < num_of_packages; i++) {
    int x, y, z, prio, w, cost;
    cin >> x >> y >> z >> prio >> w >> cost;
    packages.emplace_back(Position{x, y, z}, w, cost);
  }

  auto [solutions, costs] = compute_solutions(packages, vehicles);

  if (!check_solution_feasibility(solutions, packages, vehicles)) {
    cout << "Solution not feasible! sths wrong!";
    return 0;
  }

  cout << solutions.size() << " " << costs << '\n';

  for (auto sol : solutions) {
    cout << sol << '\n';
  }
}
