import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("output/benchmark.csv")

plt.figure(figsize=(10, 6))

for mode in data["mode"].unique():
    mode_data = data[data["mode"] == mode]
    avg_time = mode_data.groupby("n")["walltime"].mean().reset_index()
    stddev_time = mode_data.groupby("n")["walltime"].std().reset_index()
    plt.errorbar(avg_time["n"], avg_time["walltime"], yerr=stddev_time["walltime"], label=mode, capsize=5)

plt.xlabel("Number of particles")
plt.ylabel("Walltime (s)")
plt.legend()
plt.grid()
plt.yscale("log")
plt.xscale("log")
plt.savefig("results/benchmark.pdf")