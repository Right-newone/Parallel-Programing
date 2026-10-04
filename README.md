# Parallel-Programing

Лабораторная работа №1 по дисциплине «Параллельное программирование».

Реализация умножения квадратных матриц двумя способами:
- **последовательно** (классический тройной цикл);
- **параллельно** с использованием `std::thread` (разбиение по строкам результата).

Проект использует **модули C++23** (`import std;`, `export module ...`) и собирается
через **CMake + Ninja**.

---

## 📁 Структура проекта

```
.
├── Lab1/
│   ├── main.cpp                  # точка входа, замеры времени, вывод
│   ├── matrix_core_n_files.cppm          # класс Matrix, загрузка/сохранение, лог бенчмарков
│   ├── matrix_ops.cppm           # последовательное и параллельное умножение 
│   ├── gen.py                    # генератор входных матриц
│   ├── check.py                  # проверка через NumPy
│   ├── perf.py                   # построение графиков
│   ├── requirements.txt          # Python-зависимости
│   └── CMakeLists.txt
├── CMakeLists.txt
└── README.md
```

---

##  Требования

| Инструмент | Версия |
|---|---|
| CMake | 3.30+ (для `import std` — 4.3+) |
| Компилятор | MSVC 19.36+ / GCC 14+ / Clang 17+ (с поддержкой C++23) |
| Ninja | любая актуальная |
| Python | 3.10+ |

Проверено на:
- **Windows 11**, Visual Studio 2026 (MSVC 19.51), CMake 4.3, Ninja
- C++23, `import std;` через `CMAKE_EXPERIMENTAL_CXX_IMPORT_STD`

---

##  Сборка и запуск

### 1. Установить Python-зависимости

```bash
pip install -r Lab1/requirements.txt
```

или вручную:

```bash
pip install numpy matplotlib
```

### 2. Сгенерировать входные матрицы

Скрипт создаст `matrix_a.txt` и `matrix_b.txt` (по умолчанию 300×300, до 2000*2000):

```bash
cd Lab1
py gen.py           # Windows
# python3 gen.py    # Linux / macOS
```

Чтобы изменить размер, отредактируйте `SIZE` в начале `gen.py`.

### 3. Собрать C++ проект

Из корневой папки:

```bash
cmake -B build -S . -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

> **Важно:** для работы `import std;` нужен CMake 4.3+ и переменная
> `CMAKE_EXPERIMENTAL_CXX_IMPORT_STD` с актуальным UUID (она уже прописана
> в `CMakeLists.txt`).

### 4. Запустить программу

Скопируйте `.exe` рядом со скриптами (удобно для проверки):

```bash
# Windows
copy build\Lab1\Lab1\Lab1.exe Lab1\Lab1.exe

# Linux / macOS
cp build/Lab1/Lab1/Lab1 Lab1/Lab1
```

Затем:

```bash
cd Lab1
./Lab1.exe        # Windows
# ./Lab1          # Linux / macOS
```

Программа выведет что-то вроде:

```
Parallel Programming - Lab1
Matrix multiplication

Matrix size: 300 x 300
Threads: 12

Sequential time: 130.24 ms
Parallel time:    27.24 ms
Speedup:         4.78
C++ check: OK

Result: result.txt
Benchmark: benchmark.csv
```

Создаются два файла:
- `result.txt` — матрица-результат (формат совпадает с входными);
- `benchmark.csv` — строка с замером времени (дописывается при каждом запуске).

### 5. Проверить результат через NumPy

```bash
py check.py
```

Ожидаемый вывод:

```
Python / NumPy verification
---------------------------
Maximum absolute difference: 0.000e+00
RESULT: OK
```

Скрипт читает `matrix_a.txt`, `matrix_b.txt`, `result.txt`, умножает
A на B через NumPy и сравнивает с результатом C++.

### 6. Построить графики производительности

```bash
py perf.py
```

Создаётся `performance.png` с двумя графиками:
- время последовательного и параллельного умножения от размера матрицы;
- ускорение (speedup) относительно числа потоков.

`benchmark.csv` накапливает строки, и график становится наглядным.
