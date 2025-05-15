# This script visualizes the current and best-ever performance of the genetic algorithm.

import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("utils/stats.csv")

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