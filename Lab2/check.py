from pathlib import Path
import sys
import numpy as np


def load_matrix(path: Path) -> np.ndarray:
    with path.open("r", encoding="utf-8") as f:
        n = int(f.readline())
        values = []
        for line in f:
            values.extend(float(v) for v in line.split())
    if len(values) != n * n:
        raise ValueError(f"{path}: expected {n * n}, got {len(values)}")
    return np.array(values, dtype=np.float64).reshape(n, n)


def main() -> int:
    root = Path(__file__).resolve().parent
    sizes = [200, 400, 800, 1200, 1600, 2000]

    all_ok = True

    for size in sizes:
        a_path = root / f"matrix_{size}_a.txt"
        b_path = root / f"matrix_{size}_b.txt"
        r_path = root / f"result_{size}.txt"

        if not a_path.exists() or not b_path.exists():
            print(f"[SKIP] missing input for size {size}")
            all_ok = False
            continue

        if not r_path.exists():
            print(f"[FAIL] missing result file: {r_path.name}")
            all_ok = False
            continue

        a = load_matrix(a_path)
        b = load_matrix(b_path)
        cpp = load_matrix(r_path)
        reference = a @ b

        diff = float(np.max(np.abs(cpp - reference)))

        ok = np.allclose(cpp, reference, rtol=1e-9, atol=1e-9)
        status = "OK" if ok else "FAILED"
        print(f"[{status}] size={size:>5}  max|diff|={diff:.3e}")

        all_ok = all_ok and ok

    print()
    print("RESULT:", "ALL OK" if all_ok else "SOME FAILED")
    return 0 if all_ok else 2


if __name__ == "__main__":
    sys.exit(main())