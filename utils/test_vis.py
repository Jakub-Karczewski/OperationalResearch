import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from operator import itemgetter


# Path do folderu z testami
SMALL_TESTS = True
FOLDER = os.path.join(os.path.dirname(__file__), ".." if not SMALL_TESTS else ".", "knapsack_tests" if not SMALL_TESTS else "tests")

algorithms = ["Genetic", "Heuristic", "Knapsack"]
results = []
costs = []
NUM_TESTS = 10


for filename in sorted(os.listdir(FOLDER)):
    if filename.endswith(".txt") and int(filename[-5]) < NUM_TESTS:
        filepath = os.path.join(FOLDER, filename)
        row = {"Test Case": "6"}
        price = {"Test Case": "5"}

        current_algorithm = None

        with open(filepath, 'r') as file:
            for line in file:
                line = line.strip()

                for alg in algorithms:
                    if line.startswith(f"{alg} Output:"):
                        current_algorithm = alg

                if line.startswith("Total profit:") and current_algorithm:
                    cost_str = line.split(":")[1].strip()
                    price[current_algorithm] = int(cost_str)

                if line.startswith("Computation time:") and current_algorithm:
                    try:
                        time_str = line.split(":")[1].strip().rstrip("s")
                        if not SMALL_TESTS:
                            if current_algorithm == "Genetic":
                                row["Heuristic"] = float(time_str)
                            elif current_algorithm == "Heuristic":
                                row["Genetic"] = float(time_str)
                            else:
                                row["Knapsack"] = float(time_str)
                        else:
                            row[current_algorithm] = float(time_str)
                        current_algorithm = None
                    except (IndexError, ValueError):
                        continue

        results.append(row)
        costs.append(price)

results_sorted = sorted(results, key=itemgetter('Genetic'))
k = 20
if SMALL_TESTS:
    k = 4
new_results = []
for my_dict in results_sorted:
    temp = {"Test Case": f"({k}, {2 if k < 24 else (3 if k < 27 else 4)})", "Genetic": my_dict["Genetic"],
            "Heuristic": my_dict["Heuristic"]}
    if SMALL_TESTS:
        temp["Test Case"] = f"({k if k < 9 else k - 4}, {2 if k < 9 else 3})"
        temp["Knapsack"] = my_dict["Knapsack"]
    new_results.append(temp)
    k += 1

df = pd.DataFrame(new_results).sort_values("Genetic")

df = df.melt(id_vars="Test Case", var_name="Algorithm", value_name="Time (s)")


plt.figure(figsize=(12, 10))
sns.set_style("whitegrid")
plot = sns.barplot(data=df, x="Test Case", y="Time (s)", hue="Algorithm")


# ustawienie osi Y skali logarytmicznej
plot.set_yscale('log')

plot.set(xlabel='TestCase (number of packages, number of vehicles)')

for p in plot.patches:
    height = p.get_height()
    if not pd.isna(height):
        plot.annotate(f'{height:.2f}',
                      (p.get_x() + p.get_width() / 2., height),
                      ha='center', va='bottom',
                      fontsize=8, color='black', xytext=(0, 3),
                      textcoords='offset points')

plt.title("Computation Time Comparison per Test Case")
plt.xticks(rotation=45)
plt.tight_layout()

out_path = os.path.join(os.path.dirname(__file__), "computation_times1.png")
plt.savefig(out_path)
plt.clf()

plt.figure(figsize=(10, 10))

costs_sorted = sorted(costs, key=itemgetter('Genetic'))

k = 20
if SMALL_TESTS:
    k = 4
for my_dict in costs_sorted:
    if not SMALL_TESTS:
        my_dict["Test Case"] = f"({k}, {2 if k < 24 else (3 if k < 27 else 4)})"
    else:
        my_dict["Test Case"] = f"({k if k < 9 else k - 4}, {2 if k < 9 else 3})"
    k += 1

df_costs = pd.DataFrame(costs_sorted)
df_costs = df_costs.melt(id_vars="Test Case", var_name="Algorithm", value_name="Cost")

plot_cost = sns.barplot(data=df_costs, x="Test Case", y="Cost", hue="Algorithm")

plot_cost.set_yscale('log')

plt.title("Calculated profit per Test Case")

plot_cost.set(xlabel='TestCase (number of packages, number of vehicles)')

for p in plot_cost.patches:
    height = p.get_height()
    if not pd.isna(height):
        plot_cost.annotate(f'{height:.2f}',
                      (p.get_x() + p.get_width() / 2., height),
                      ha='center', va='bottom',
                      fontsize=8, color='black', xytext=(0, 3),
                      textcoords='offset points')

out_path = os.path.join(os.path.dirname(__file__), "costs2.png")
plt.savefig(out_path)
