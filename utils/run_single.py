"""
Runs the genetic algorithm solver with a specified strategy and input file multiple times.

Can be used to compare crossover strategies.

Usage:
    python run_single.py <strategy> <run_id>
Arguments:
    strategy: Crossover strategy to be used ("uniform", "heuristic", "one_point).
    run_id: Identifier for the test run, used to select the input file.

Input file comes from tests/test{input_id}.txt. If it doesn't exist, run generate_input.py first.
"""
import copy
import subprocess
import sys
import os
import csv
from time import time


def run_solver(header_, strategy, params, run_id, input_file, out_csv="table_params.csv",
               out_dir="results_run_single"):
    with open(input_file, "r") as f:
        input_data = f.read()

    # Dodanie strategii krzyżowania do inputu
    full_input = strategy + f"\n{params[0]} {params[1]} {params[2]} {params[3]}" + "\n" + input_data
    time1 = time()
    result = subprocess.run(
        ["../genetic_solve.out"],
        input=full_input.encode(),
        capture_output=True,
    )
    time2 = time()
    total_time = round(time2 - time1, 2)

    os.makedirs(out_dir, exist_ok=True)

    output_file = os.path.join(out_dir, f"genetic_{strategy}_run{run_id}.txt")
    output_csv = os.path.join(os.path.dirname(__file__), ".", out_csv)

    with open(output_file, 'w') as f:
        f.write(result.stdout.decode())
        f.write('\n')
    cost_str = ""
    with open(output_file, "r") as f:
        for line in f:
            # print(line)
            line = line.strip()
            if line.startswith("Total profit:"):
                print(line)
                cost_str = line.split(":")[1].strip()

    data = [{"cross_type": strategy, "pop_size": params[0], "child_perc": params[1], "mut_perc": params[2],
             "gen_mut_perc": params[3], "cost": int(cost_str), "exec_time": total_time}]

    with open(output_csv, 'a', newline='') as csvfile:
        fieldnames = ['cross_type', 'pop_size', 'child_perc', 'mut_perc', 'gen_mut_perc', 'cost', 'exec_time']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        if header_:
            writer.writeheader()
        writer.writerows(data)


NUM_TESTS = 5
if __name__ == "__main__":

    # strategy = sys.argv[1] if len(sys.argv) > 1 else "uniform"
    # uniform, one_point, heuristic
    # run_id = sys.argv[2] if len(sys.argv) > 2 else "0"
    run_id = "0"

    input_file = f"test_inputs/input_{run_id}.txt"
    if not os.path.exists(input_file):
        print(f"Plik wejściowy {input_file} nie istnieje.")
        sys.exit(1)
    head = False

    start_params = [[0, 30, 25, 40], [0, 60, 15, 40], [0, 60, 25, 20]]
    params_steps = [[0, 10, 0, 0], [0, 0, 10, 0], [0, 0, 0, 10]]
    for z, population in enumerate([50, 100, 150, 200, 250]):
        for l, strategy in enumerate(["uniform", "one_point", "heuristic"]):
            for k in range(len(params_steps)):
                for i in range(NUM_TESTS):
                    params = copy.deepcopy(start_params[k])
                    params[0] = population
                    for j in range(1, len(params)):
                        params[j] += i * params_steps[k][j]
                    print(z, l, k, i)
                    run_solver(False if z + k + i + l > 0 else True, strategy, params, run_id, input_file)
