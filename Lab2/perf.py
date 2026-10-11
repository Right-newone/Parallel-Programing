from pathlib import Path
import csv
from collections import defaultdict

import matplotlib.pyplot as plt
import numpy as np


def main() -> int:
    root = Path(__file__).resolve().parent
    csv_file = root / "benchmark.csv"

    if not csv_file.exists():
        print("benchmark.csv not found. Run Lab2.exe first.")
        return 1

    
    data = defaultdict(dict)

    with csv_file.open("r", encoding="utf-8") as f:
        for row in csv.DictReader(f):
            size = int(row["matrix_size"])
            threads = int(row["threads"])
            seq = float(row["sequential_ms"])
            par = float(row["parallel_ms"])
            sp = float(row["speedup"])
            data[size][threads] = (seq, par, sp)

    sizes = sorted(data.keys())
    threads_all = sorted({t for s in sizes for t in data[s]})

    
    fig, ax = plt.subplots(figsize=(9, 6))
    for size in sizes:
        ts = sorted(data[size].keys())
        ys = [data[size][t][1] for t in ts]
        ax.plot(ts, ys, "o-", label=f"N={size}")
    ax.set_xlabel("Threads")
    ax.set_ylabel("Parallel time (ms)")
    ax.set_title("Parallel time vs Threads")
    ax.set_xscale("log", base=2)
    ax.set_xticks(threads_all)
    ax.set_xticklabels([str(t) for t in threads_all])
    ax.grid(True, which="both", alpha=0.3)
    ax.legend()
    plt.tight_layout()
    plt.savefig(root / "time_vs_threads.png", dpi=120)
    plt.close(fig)

    
    fig, ax = plt.subplots(figsize=(9, 6))
    for size in sizes:
        ts = sorted(data[size].keys())
        ys = [data[size][t][2] for t in ts]
        ax.plot(ts, ys, "^-", label=f"N={size}")

    
    if threads_all:
        ax.plot(
            threads_all,
            threads_all,
            "k--",
            label="ideal (linear)",
        )

    ax.set_xlabel("Threads")
    ax.set_ylabel("Speedup")
    ax.set_title("Speedup vs Threads")
    ax.set_xscale("log", base=2)
    ax.set_xticks(threads_all)
    ax.set_xticklabels([str(t) for t in threads_all])
    ax.grid(True, which="both", alpha=0.3)
    ax.legend()
    plt.tight_layout()
    plt.savefig(root / "speedup_vs_threads.png", dpi=120)
    plt.close(fig)

    
    if threads_all:
        matrix = np.full((len(sizes), len(threads_all)), np.nan)
        for i, size in enumerate(sizes):
            for j, t in enumerate(threads_all):
                if t in data[size]:
                    matrix[i, j] = data[size][t][2]

        fig, ax = plt.subplots(figsize=(9, 6))
        im = ax.imshow(matrix, aspect="auto", cmap="viridis")
        ax.set_xticks(range(len(threads_all)))
        ax.set_xticklabels([str(t) for t in threads_all])
        ax.set_yticks(range(len(sizes)))
        ax.set_yticklabels([str(s) for s in sizes])
        ax.set_xlabel("Threads")
        ax.set_ylabel("Matrix size N")
        ax.set_title("Speedup heatmap")
        fig.colorbar(im, ax=ax, label="Speedup")
        plt.tight_layout()
        plt.savefig(root / "speedup_heatmap.png", dpi=120)
        plt.close(fig)

    print("Saved:")
    print("  time_vs_threads.png")
    print("  speedup_vs_threads.png")
    print("  speedup_heatmap.png")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())