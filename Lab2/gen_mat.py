import random
from pathlib import Path

sizes = [200, 400, 800, 1200, 1600, 2000]

random_generator = random.Random(20261011)
directory = Path(__file__).resolve().parent

for size in sizes:
    for suffix in ("a", "b"):
        file_path = directory / f"matrix_{size}_{suffix}.txt"

        with file_path.open("w", encoding="ascii", newline="\n") as file:
            file.write(f"{size}\n")

            for _ in range(size):
                row = [
                    str(random_generator.randint(0, 9))
                    for _ in range(size)
                ]
                file.write(" ".join(row) + "\n")

        print(f"Created: {file_path.name}")