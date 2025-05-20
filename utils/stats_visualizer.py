# This script visualizes the current and best-ever performance of the genetic algorithm.

import pandas as pd
import matplotlib.pyplot as plt
import os

df = pd.read_csv("../utils/stats.csv")

plt.plot(df["generation"], df["current_best"], label="Current Best")
plt.plot(df["generation"], df["best_ever"], label="Best Ever", linestyle='--')

plt.xlabel("Generation")
plt.ylabel("Cost")
plt.title("Genetic Algorithm: Current vs Best Ever")
plt.legend()
plt.grid(True)
plt.tight_layout()
out_path = os.path.join(os.path.dirname(__file__), "genetic_times.png")
plt.savefig(out_path)
#plt.show()