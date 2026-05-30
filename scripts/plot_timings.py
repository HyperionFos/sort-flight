import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("data/timings.csv")

algorithms = df["algorithm"].unique()

# --- Linear scale ---
fig, ax = plt.subplots(figsize=(9, 6))
for algo in algorithms:
    sub = df[df["algorithm"] == algo]
    ax.plot(sub["size"], sub["time_ms"], marker="o", label=algo)

ax.set_xlabel("Array size")
ax.set_ylabel("Time, ms")
ax.set_title("Sorting time vs array size (linear scale)")
ax.legend()
ax.grid(True, linestyle="--", alpha=0.5)
plt.tight_layout()
plt.savefig("data/plot_linear.png", dpi=150)
plt.close()

# --- Log scale ---
fig, ax = plt.subplots(figsize=(9, 6))
for algo in algorithms:
    sub = df[df["algorithm"] == algo]
    ax.plot(sub["size"], sub["time_ms"], marker="o", label=algo)

ax.set_xscale("log")
ax.set_yscale("log")
ax.set_xlabel("Array size (log)")
ax.set_ylabel("Time, ms (log)")
ax.set_title("Sorting time vs array size (log-log scale)")
ax.legend()
ax.grid(True, which="both", linestyle="--", alpha=0.5)
plt.tight_layout()
plt.savefig("data/plot_log.png", dpi=150)
plt.close()

print("saved data/plot_linear.png and data/plot_log.png")