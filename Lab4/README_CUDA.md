# CUDA matrix multiplication laboratory

## Files

- `main.cpp` — CPU reference multiplication, CUDA configuration experiments, result checking and benchmark output.
- `matrix_core_n_files.cppm` — matrix storage, input/output and CSV logging.
- `matrix_ops.cppm` — unchanged CPU sequential and `std::thread` implementations from Lab1.
- `matrix_cuda.hpp` — plain C interface between C++23 and CUDA translation units.
- `matrix_cuda.cu` — CUDA kernel, device memory transfers and CUDA event timing.
- `CMakeLists.txt` — C++23 modules plus CUDA C++20, targeting RTX 3080 (`sm_86`).
- `gen.py` — creates two matrices of a requested size.
- `check.py` — verifies `result.txt` with NumPy.
- `perf.py` — plots CPU/CUDA time, kernel time and speedup.
- `run_all.ps1` — experiments for sizes 200, 400, 800, 1200, 1600 and 2000.

## Build

Run these commands from this folder in **Developer PowerShell for Visual Studio 2026**:

```powershell
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

If Ninja is not installed, use the Visual Studio generator instead:

```powershell
cmake -S . -B build -G "Visual Studio 18 2026" -A x64
cmake --build build --config Release
```

## One test

```powershell
py .\gen.py --size 200
.\build\LabCUDA.exe
py .\check.py
```

For the Visual Studio generator, the executable is normally under `build\Release\LabCUDA.exe`.

## All experiments

After building, run:

```powershell
.\run_all.ps1
py .\perf.py
```

The program tests four block shapes: 8x8, 16x16, 32x8 and 32x32. Each configuration is warmed up and then measured three times. `cuda_kernel_ms` records CUDA-event time for the kernel alone. `cuda_total_ms` records host elapsed time for allocation, host-to-device copies, kernel execution and device-to-host copy; context initialization is performed before measurements. The reported speedup uses the total CUDA time.

`result.txt` contains the result from the configuration with the lowest measured total CUDA time. `benchmark.csv` contains one row per matrix size and block configuration.
