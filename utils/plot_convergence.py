"""
This scipt plots the convergence of a genetic algorithm.

Usage:
    python plot_convergence.py <input_id>
Arguments:
    input_id: Identifier for the test case, used to select the CSV file.

CSV file comes from tests/test{input_id}.csv. If it doesn't exist, run test_generator.py first.
"""

import pandas as pd
import matplotlib.pyplot as plt
import sys

input_id = sys.argv[1] if len(sys.argv) > 1 else "0"

df = pd.read_csv(f"./tests/test{input_id}.csv")

final_best = df["best_ever"].iloc[-1]
first_hit_row = df[df["current_best"] == final_best].iloc[0]
first_hit_generation = first_hit_row["generation"]

plt.plot(df["generation"], df["current_best"], label="Current Best")
plt.plot(df["generation"], df["best_ever"], label="Best Ever", linestyle='--')

plt.axvline(x=first_hit_generation, color='red', linestyle=':', linewidth=1.5, label=f"First Generation of Best Ever ({first_hit_generation})")

plt.xlabel("Generation")
plt.ylabel("Cost")
plt.title("Genetic Algorithm: Current vs Best Ever")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()