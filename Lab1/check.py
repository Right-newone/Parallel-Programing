from pathlib import Path
import subprocess
import sys
import numpy as np

def load_matrix(filename):
    with filename.open("r", encoding="utf-8") as file:
        n = int(file.readline())
        values = []
        for line in file:
            values.extend(float(v) for v in line.split())
    if len(values) != n * n:
        raise ValueError(f"{filename}: expected {n * n} values, got {len(values)}")
    return np.array(values, dtype=np.float64).reshape(n, n)

def find_executable(root):
    candidates = [
        root / "Lab1.exe",
        root / "Lab1",
        root.parent / "build" / "Lab1" / "Lab1.exe",
        root.parent / "build" / "Lab1" / "Lab1",
        root / "MatrixParallelLab",
        root / "MatrixParallelLab.exe",
        root / "build" / "MatrixParallelLab",
        root / "build" / "MatrixParallelLab.exe",
        root / "build" / "Debug" / "MatrixParallelLab.exe",
        root / "out" / "build" / "x64-Debug" / "MatrixParallelLab.exe",
    ]
    return next((p for p in candidates if p.exists()), None)

def main():
    root = Path(__file__).resolve().parent

    if not (root / "matrix_a.txt").exists() or not (root / "matrix_b.txt").exists():
        print("ERROR: input matrices are missing. Run gen.py first.")
        return 1

    result_file = root / "result.txt"
    if not result_file.exists():
        print("ERROR: result.txt not found. Run Lab1.exe first (from this folder).")
        return 1

    a = load_matrix(root / "matrix_a.txt")
    b = load_matrix(root / "matrix_b.txt")
    cpp = load_matrix(result_file)
    reference = a @ b

    diff = np.max(np.abs(cpp - reference))
    print()
    print("Python / NumPy verification")
    print("---------------------------")
    print(f"Maximum absolute difference: {diff:.3e}")

    if np.allclose(cpp, reference, rtol=1e-9, atol=1e-9):
        print("RESULT: OK")
        return 0

    print("RESULT: FAILED")
    return 2
if __name__ == "__main__":
    sys.exit(main())