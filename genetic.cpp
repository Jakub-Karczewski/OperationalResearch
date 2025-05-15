#include <bits/stdc++.h>
#include "bo_base.h"
#include "crossovers/uniform_crossover.h"
#include "crossovers/one_point_crossover.h"
#include "crossovers/heuristic_crossover.h"

#define POPULATION_SIZE 200
#define CHILDREN_PERCENTAGE 80
#define MUTATION_PERCENTAGE 25
#define GENE_MUTATION_PERCENTAGE 40

using namespace std;

using Assignment = vector<int>;

struct GenerationStats {
    int generation;
    int current_best;
    int best_ever;
};

ostream& operator<<(ostream& os, Assignment as) {
    for(auto x:as) {
        os << x << ' ';
    }
    return os;
}

Assignment generate_random_assignment(int num_of_packages, int num_of_vehicles) {
    // Generuje losowy wektor assignment
    Assignment assignment(num_of_packages);
    for (int i = 0; i < num_of_packages; i++) {
        assignment[i] = rand() % num_of_vehicles;
    }
    return assignment;
}

// Uzywa wczesniej zaimplementowanej heurystyki do ulozenia paczek we wskazanych pojazdach.
pair<vector<Solution>, int> build_solution_from_assignment(const Assignment& assignment, const vector<Package>& packages, const vector<Vehicle>& vehicles) {
    vector<vector<Package>> packages_per_vechicles(vehicles.size());
    vector<vector<int>> package_ids_per_vechicles(vehicles.size());
    for(int i=0;i<assignment.size();i++) {
        packages_per_vechicles[assignment[i]].push_back(packages[i]);
        package_ids_per_vechicles[assignment[i]].push_back(i);
    }

    pair<vector<Solution>, int> solutions;
    for(int i=0;i<vehicles.size();i++) {
        if(packages_per_vechicles[i].size()>0) {
            vector<Vehicle> tmp1 = {vehicles[i]};
            tuple<vector<Solution>, int> tmp2 = compute_solutions(packages_per_vechicles[i], tmp1);
            auto [computed_solutions, cost] = tmp2;
            solutions.second += cost;
            for(int j=0;j<computed_solutions.size();j++) {
                computed_solutions[j].vehicle_id = i;
                computed_solutions[j].package_id = package_ids_per_vechicles[i][j];
            }

            solutions.first.insert(solutions.first.begin(), computed_solutions.begin(), computed_solutions.end());
        }
    }

    return solutions;
}

Assignment mutate_assignment(const Assignment& assignment, int num_of_vehicles) {
    Assignment mutated_assignment = assignment;

    for(int i=0;i<assignment.size();i++) {
        if(rand() %  100 < GENE_MUTATION_PERCENTAGE) {
            mutated_assignment[i] = rand() % num_of_vehicles;
        }
    }
    return mutated_assignment;
}

vector<Assignment> initialize_population(int population_size, int num_packages, int num_vehicles) {
    vector<Assignment> population(population_size);
    for (int i = 0; i < population_size; ++i) {
        population[i] = generate_random_assignment(num_packages, num_vehicles);
    }
    return population;
}

void sort_population_by_evaluation_in_place(vector<Assignment> population, const vector<Package>& packages, const vector<Vehicle>& vehicles) {
    // Przyjmuje populację assignmentów i ocenia je (liczy profit z paczek), zwracając posortowane
    vector<pair<int, Assignment>> scored_population; // pary <profit, assignment>
    for (const auto& assignment : population) {
        auto [solutions, cost] = build_solution_from_assignment(assignment, packages, vehicles);
        // Heura dziala tak, ze zawsze generuje rozwiazanie poprawne. Moze ono odrzucac paczki bez wyrzucania bledow.
        scored_population.emplace_back(cost, assignment);
    }

    sort(scored_population.rbegin(), scored_population.rend());

    population.clear();
    for(auto &[cost, assignment]:scored_population) {
        population.emplace_back(move(assignment));
    }
}

vector<Assignment> reproduce_best(const vector<Assignment>& best_assignments, int population_size, int num_vehicles, const vector<Package>& packages, const vector<Vehicle>& vehicles) {
    // Przyjmuje najlepsze assignmenty i krzyżuje je, zwraca populację dzieci
    vector<Assignment> new_population;

    for(int i=0;i<population_size * (100 - CHILDREN_PERCENTAGE) / 100;i++) {
        new_population.push_back(best_assignments[i]);
    }

    mt19937 rng(random_device{}());
    cauchy_distribution<> dist(0.0, 1.0);
    const double center = static_cast<double>(best_assignments.size() - 1) / 2.0;
    const double scale = center / 3.0;

    while(new_population.size() < population_size) {
        double cauchy_sample_1 = dist(rng);
        double cauchy_sample_2 = dist(rng);
        double raw_index_1 = center + scale * cauchy_sample_1;
        double raw_index_2 = center + scale * cauchy_sample_2;
        int index_1 = static_cast<int>(round(raw_index_1));
        int index_2 = static_cast<int>(round(raw_index_2));
        index_1 = clamp(index_1, 0, static_cast<int>(best_assignments.size()) - 1);
        index_2 = clamp(index_2, 0, static_cast<int>(best_assignments.size()) - 1);
        
        // Topornie sie ustawia, trzeba to poprawic
        //static UniformCrossover crossover; // UniformCrossover, OnePointCrossover
        HeuristicCrossover crossover(packages, vehicles);
        Assignment child_assignment = crossover.crossover(best_assignments[index_1], best_assignments[index_2]);

        if (rand() % 100 < MUTATION_PERCENTAGE) {
            child_assignment = mutate_assignment(child_assignment, num_vehicles);
        }
        new_population.push_back(child_assignment);
    }
    return new_population;
}

tuple<vector<Solution>, int> genetic_algorithm(const vector<Package>& packages, const vector<Vehicle>& vehicles) {
    const int generations = 25;
    const int num_packages = packages.size();
    const int num_vehicles = vehicles.size();

    vector<GenerationStats> generations_stats;
    pair<Assignment, int> best_ever{};

    vector<Assignment> population = initialize_population(POPULATION_SIZE, num_packages, num_vehicles);

    for (int gen = 0; gen < generations; gen++) {
        sort_population_by_evaluation_in_place(population, packages, vehicles);

        auto [best_solutions, current_best_cost] = build_solution_from_assignment(population[0], packages, vehicles);
        if (current_best_cost > best_ever.second) {
            best_ever = {population[0], current_best_cost};
        }

        generations_stats.push_back({gen, current_best_cost, best_ever.second});
        population = reproduce_best(population, POPULATION_SIZE, num_vehicles, packages, vehicles);
    }

    // Zapis statystyk do pliku
    ofstream stats_file("utils/stats.csv");
    stats_file << "generation,current_best,best_ever\n";
    for (const auto& s : generations_stats) {
        stats_file << s.generation << "," << s.current_best << "," << s.best_ever << "\n";
    }

    return build_solution_from_assignment(best_ever.first, packages, vehicles);
}

int main() {
    int num_of_vehicles, num_of_packages;
    srand(time(0));

    cin >> num_of_vehicles;
    vector<Vehicle> vehicles;

    for (int i = 0; i < num_of_vehicles; i++) {
        int x, y, z, w;
        cin >> x >> y >> z >> w;
        vehicles.emplace_back(Position{ x, y, z }, w);
    }

    cin >> num_of_packages;
    vector<Package> packages;

    for (int i = 0; i < num_of_packages; i++) {
        int x, y, z, prio, w, cost;
        cin >> x >> y >> z >>  w >> cost;
        packages.emplace_back(Position{ x, y, z }, w, cost);
    }

    auto [solutions, cost] = genetic_algorithm(packages, vehicles);

    cout << "Best solution found: " << endl;
    sort(solutions.begin(), solutions.end(), [&](Solution a, Solution b){ return a.package_id < b.package_id;});
    for (const auto& sol : solutions) {
        cout << sol << endl;
    }
    cout << "Total profit: " << cost << endl;
}