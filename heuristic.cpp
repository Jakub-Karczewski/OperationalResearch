#include <bits/stdc++.h>
#include "bo_base.h"

using namespace std;


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
    packages.emplace_back(Position{x, y, z}, w, cost, i);
  }


  auto [solutions, costs] = compute_solutions(packages, vehicles);

  if (!check_solution_feasibility(solutions, packages, vehicles)) {
    cout << "Solution not feasible! sths wrong!";
    return 0;
  }

    cout << "Best solution found: " << endl;
    cout << solutions.size() << endl;

  for (auto sol : solutions) {
    cout << sol << '\n';
  }
  cout << "Total profit: " << costs << endl;
}
