from datetime import datetime, timedelta
import random
import subprocess
import os
import shutil

"""
rodzaj_krzyzowania (one_point/uniform/heuristic)
<liczba_vehicli>
x y z weight_limit
[...]

<liczba_paczek>
x y z priority weight cost
[...]
"""

# STALE DO GENEROWANIA INPUTU

NUM_VEHICLES = 2
NUM_PACKAGES = 7
CONST_PRIOR = True
USE_SACK = False

VEHICLE_SIZE_RANGE = (1, 7)
WEIGHT_LIMIT_RANGE = (50, 200)

PACKAGE_SIZE_RANGE = (1, 3)
WEIGHT_RANGE = (5, 30)
PRIORITY_RANGE = (1, 5)
COST_RANGE = (10, 100)
NUM_TESTS = 5


def generate_position(size_range):
    return (
        random.randint(*size_range),
        random.randint(*size_range),
        random.randint(*size_range),
    )


def generate_vehicles(num_vehicles):
    vehicles = []
    for _ in range(num_vehicles):
        x, y, z = generate_position(VEHICLE_SIZE_RANGE)
        weight_limit = random.randint(*WEIGHT_LIMIT_RANGE)
        vehicles.append(((x, y, z), weight_limit))
    return vehicles


def generate_packages(num_packages):
    packages = []
    for _ in range(num_packages):
        x, y, z = generate_position(PACKAGE_SIZE_RANGE)
        priority = random.randint(*PRIORITY_RANGE)
        weight = random.randint(*WEIGHT_RANGE)
        cost = random.randint(*COST_RANGE)
        packages.append(((x, y, z), priority, weight, cost))
    return packages


def generate_input():
    vehicles = generate_vehicles(NUM_VEHICLES)
    packages = generate_packages(NUM_PACKAGES)

    input = ""
    input += str(NUM_VEHICLES) + "\n"
    for (x, y, z), limit in vehicles:
        input += f"{x} {y} {z} {limit}\n"

    input += str(NUM_PACKAGES) + "\n"
    for (x, y, z), prio, w, cost in packages:
        input += f"{x} {y} {z} {1 if CONST_PRIOR else prio} {w} {cost}\n"

    return input


if __name__ == "__main__":
    os.makedirs("./tests", exist_ok=True)

    for i in range(NUM_TESTS):
        print(i)
        input = generate_input()
        
        start_heur = datetime.now()
        output_heuristic = subprocess.run(
            [
                "../heuristic_solve.out"
            ],
            input=input.encode(),
            capture_output=True,
        )
        end_heur = datetime.now()
        heur_duration = end_heur-start_heur

        crossing_type = "uniform"
        input_genetic = f"{crossing_type}\n" + input # crossover strategy, hardcoded!
        start_gen = datetime.now()
        output_genetic = subprocess.run(
            [
                "../genetic_solve.out"
            ],
            input=input_genetic.encode(),
            capture_output=True,
        )
        end_gen = datetime.now()
        gen_duration = end_gen-start_gen

        # copy csv file only for genetic algorithm
        shutil.copyfile("../utils/stats.csv", f"./tests/test{i}.csv")

        if USE_SACK:
            start_sack = datetime.now()
            output_sack = subprocess.run(
                [
                    "../knapsack_solve.out"
                ],
                input=input.encode(),
                capture_output=True,
            )
            end_sack = datetime.now()
            sack_duration = end_sack-start_sack

        with open(f"./tests/test{i}.txt", "w") as file:
            file.write("===TEST CASE===\n")
            file.write("\nInput:\n")
            file.write(input + "\n")
            file.write("\nGenetic Output:\n")
            file.write(output_genetic.stdout.decode() + "\n")
            file.write(f"\nComputation time: {gen_duration.total_seconds():.2f}s" + "\n")
            file.write("\nHeuristic Output:\n")
            file.write(output_heuristic.stdout.decode() + "\n")
            file.write(f"\nComputation time: {heur_duration.total_seconds():.2f}s" + "\n")

            if USE_SACK:
                file.write("\nKnapsack Output:\n")
                file.write(output_sack.stdout.decode() + "\n")
                file.write(f"\nComputation time: {sack_duration.total_seconds():.2f}s" + "\n")