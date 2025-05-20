"""
Generates NUM_TESTS test input files and saves them in the test_inputs directory.
"""

import random
import os

NUM_VEHICLES = 5
NUM_PACKAGES = 20
CONST_PRIOR = True

VEHICLE_SIZE_RANGE = (1, 20)
WEIGHT_LIMIT_RANGE = (50, 200)

PACKAGE_SIZE_RANGE = (1, 7)
WEIGHT_RANGE = (5, 50)
PRIORITY_RANGE = (1, 5)
COST_RANGE = (10, 100)
NUM_TESTS = 10

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

    if not os.path.exists("test_inputs"):
        os.makedirs("test_inputs")

    for i in range(NUM_TESTS):
        with open(f"test_inputs/input_{i}.txt", "w") as f:
            f.write(generate_input())