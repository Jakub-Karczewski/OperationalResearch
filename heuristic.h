#pragma once
#include <bits/stdc++.h>
#include "bo_base.h"

using namespace std;


bool check_solution_feasibility(const vector<Solution> &solutions,
                                const vector<Package> &packages,
                                const vector<Vehicle> &vehicles);

tuple<vector<Solution>, int> compute_solutions(vector<Package> &packages,
                                               vector<Vehicle> &vehicles);


