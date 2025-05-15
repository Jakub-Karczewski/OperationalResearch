import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Path do folderu z testami
FOLDER = os.path.join(os.path.dirname(__file__), "..", "showcase_tests")
# FOLDER = os.path.join(os.path.dirname(__file__), "..", "knapsack_tests")

algorithms = ["Genetic", "Heuristic", "Knapsack"]
results = []

for filename in sorted(os.listdir(FOLDER)):
    if filename.endswith(".txt"):
        filepath = os.path.join(FOLDER, filename)
        row = {"Test Case": filename}

        current_algorithm = None

        with open(filepath, 'r') as file:
            for line in file:
                line = line.strip()

                for alg in algorithms:
                    if line.startswith(f"{alg} Output:"):
                        current_algorithm = alg

                if line.startswith("Computation time:") and current_algorithm:
                    try:
                        time_str = line.split(":")[1].strip().rstrip("s")
                        row[current_algorithm] = float(time_str)
                        current_algorithm = None
                    except (IndexError, ValueError):
                        continue

        results.append(row)
df = pd.DataFrame(results).sort_values("Test Case")
df = df.melt(id_vars="Test Case", var_name="Algorithm", value_name="Time (s)")

plt.figure(figsize=(12, 6))
sns.set_style("whitegrid")
plot = sns.barplot(data=df, x="Test Case", y="Time (s)", hue="Algorithm")

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

out_path = os.path.join(os.path.dirname(__file__), "computation_times.png")
plt.savefig(out_path)
plt.show()