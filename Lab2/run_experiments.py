from pathlib import Path
import subprocess
import shutil
import sys

root = Path(__file__).resolve().parent
exe = root / "build" / "Lab2" / "Lab2"   

core_configs = [1, 2, 4, 6]  
all_results = []

for cores in core_configs:
    csv_path = root / "benchmark.csv"
    if csv_path.exists():
        csv_path.unlink()

    if sys.platform.startswith("linux"):
        cmd = ["taskset", "-c", f"0-{cores-1}", str(exe)]
    else:
        cmd = [str(exe)]  

    print(f"\n>>> Running with {cores} core(s)")
    subprocess.run(cmd, check=True, cwd=root)

    if csv_path.exists():
        dst = root / f"benchmark_cores_{cores}.csv"
        shutil.copy(csv_path, dst)
        all_results.append(dst)
        print(f"  saved {dst.name}")

print("\nDone. Results:")
for p in all_results:
    print(" ", p.name)