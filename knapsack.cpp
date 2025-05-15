#include <bits/stdc++.h>
#include "bo_base.h"

#include "heuristic.h"



pair< long long, vector<Solution> > build_solution_from_assignment_knapsack(const vector<int> &assignment, const vector<Package> &packages,
                                                                            const vector<Vehicle> &vehicles) {
    pair< long long, vector<Solution> > solutions;
    vector<vector<Package> > divide_packages(vehicles.size(), vector<Package> ());
    vector<vector<int>> pkgs_IDs(vehicles.size(), vector<int> ());
    for (int i = 0; i < assignment.size(); ++i) {
        int vehicle_id = assignment[i];
        divide_packages[vehicle_id].push_back(packages[i]);
        pkgs_IDs[vehicle_id].push_back(i);
    }
    for (int i = 0; i < vehicles.size(); i++){
        if(divide_packages[i].empty()){
            continue;
        }
        vector<unordered_map<Position, bool>> map1(1);
        vector <int> one_vehicle_ID(1);
        vector <Vehicle> only_one(1, vehicles[i]);
        vector<long long> pref_sum(divide_packages[i].size() + 1, 0ll);
        vector<bool> vis(divide_packages[i].size());
        vector<long long > weights (1, vehicles[i].weight_limit);
        fill_map(map1[0], {0, 0, 0}, {vehicles[i].size.x, vehicles[i].size.y, vehicles[i].size.z });
        one_vehicle_ID[0] = i;

        vector<pair<Position, int>> where_placed(divide_packages[i].size(), pair(Position(-1, -1, -1), -1));
        for(int j = int(divide_packages[i].size()-1); j >= 0; j--){
            pref_sum[j] = pref_sum[j+1] + divide_packages[i][j].cost;
        }
        long long XD = 0ll;
        pair<long long, vector<Solution>> res_i = knapsack_problem(packages, where_placed, pkgs_IDs[i],
                                                                   map1, 0, 0ll, weights, pref_sum,
                                                                   vis, only_one, one_vehicle_ID);
        solutions.second.insert(solutions.second.end(), res_i.second.begin(), res_i.second.end());
        solutions.first += res_i.first;

    }
    return solutions;
}




pair<long long, vector<Solution>> knapsack_call(const vector<Package>& packages, vector<Vehicle> vehicles ){
    vector<int> example_assignments (packages.size(), 0);
    auto sol1 = build_solution_from_assignment_knapsack(example_assignments, packages, vehicles);
    return sol1;
}

pair < long long, vector<Solution>> knapsack_raw(const vector<Package>& packages, vector<Vehicle> vehicles){

    vector<unordered_map<Position, bool>> map1(vehicles.size());
    vector <int> vehicle_IDs(vehicles.size());
    vector<int> packages_IDs(packages.size());
    vector<long long> pref_sum(packages.size() + 1, 0ll);
    vector<bool> vis(packages.size(), false);
    vector<long long> weights(vehicles.size(), 0);
    vector<pair<Position, int>> where_placed(packages.size(), pair(Position(-1, -1, -1), -1));

    for(int i = 0 ; i < vehicles.size(); i++) {
            fill_map(map1[i], {0, 0, 0}, {vehicles[i].size.x, vehicles[i].size.y, vehicles[i].size.z});
            vehicle_IDs[i] = i;
        }
    for (int i = 0; i < packages.size(); i++){
        packages_IDs[i] = i;
    }

    for(int i = int(packages.size()-1); i >= 0; i--){
        pref_sum[i] = pref_sum[i+1] + packages[i].cost;
    }
    long long XD = 0ll;
    return knapsack_problem(packages, where_placed, packages_IDs, map1, 0, 0ll, weights,
                            pref_sum, vis, vehicles, vehicle_IDs);

}

int main() {
    int num_of_vehicles, num_of_packages;
    srand(time(nullptr));

    cin >> num_of_vehicles;
    vector<Vehicle> vehicles;
    vector<Vehicle> vehicles_check;

    for (int i = 0; i < num_of_vehicles; i++) {
        int x, y, z, w;
        cin >> x >> y >> z >> w;
        vehicles.emplace_back(Position{x-1, y-1, z-1}, w);
        vehicles_check.emplace_back(Position{x, y, z}, w);
    }


    cin >> num_of_packages;
    vector<Package> packages;
    vector<Package> packages_check;

    for (int i = 0; i < num_of_packages; i++) {
        int x, y, z, prio, w, cost;
        cin >> x >> y >> z >> prio >> w >> cost;
        packages.emplace_back(Position{x-1, y-1, z-1}, w, cost);
        packages_check.emplace_back(Position{x, y, z}, w, cost);
    }

    /*
    sort(packages.begin(), packages.end(),
         [](const Package &a,
            const Package &b) { // first by prio (desc) then by cost (desc)
             return b.cost <= a.cost;
         });
    */

    auto solutions = knapsack_raw(packages, vehicles);

    if(!check_solution_feasibility(solutions.second, packages_check, vehicles_check)){
        cout << "Solution is not feasible" << endl;
        return 0;
    }

    cout << "Best solution found: " << endl;
    cout << solutions.second.size() << endl;
    for (const auto &sol: solutions.second) {
        cout << sol << endl;
    }
    cout << "Total profit: " << solutions.first << endl;
    return 0;
}