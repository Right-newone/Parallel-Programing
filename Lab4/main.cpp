import std;
import matrix_core_n_files;
import matrix_ops;

namespace
{
    bool matricesEqual(
        const Mat &firstMatrix,
        const Mat &secondMatrix,
        double epsilon = 1e-9)
    {
        const std::size_t size = firstMatrix.dimension();

        if (size != secondMatrix.dimension())
        {
            return false;
        }

        for (std::size_t row = 0; row < size; ++row)
        {
            for (std::size_t column = 0; column < size; ++column)
            {
                const double difference =
                    std::abs(firstMatrix.at(row, column) -
                             secondMatrix.at(row, column));

                if (difference > epsilon)
                {
                    return false;
                }
            }
        }

        return true;
    }

    template <typename Function>
    std::pair<Mat, double> measureTime(Function &&function)
    {
        const auto begin = std::chrono::steady_clock::now();

        Mat result = function();

        const auto finish = std::chrono::steady_clock::now();

        const double elapsedMs =
            std::chrono::duration<double, std::milli>(
                finish - begin)
                .count();

        return {std::move(result), elapsedMs};
    }
}

int main()
{
    try
    {
        const Mat first = loadMat("matrix_a.txt");
        const Mat second = loadMat("matrix_b.txt");

        if (first.dimension() != second.dimension())
        {
            throw std::runtime_error(
                "Matrix dimensions must be equal.");
        }

        const std::size_t matrixSize = first.dimension();

        std::size_t workerThreads =
            std::thread::hardware_concurrency();

        if (workerThreads == 0)
        {
            workerThreads = 4;
        }

        workerThreads = std::min(workerThreads, matrixSize);

        std::cout << "Parallel Programming - Lab1\n";
        std::cout << "Matrix multiplication\n\n";
        std::cout << "Matrix size: "
                  << matrixSize << " x " << matrixSize << '\n';
        std::cout << "Threads: "
                  << workerThreads << "\n\n";

        const auto [sequentialResult, sequentialTime] =
            measureTime(
                [&]()
                {
                    return multiplySequential(first, second);
                });

        const auto [parallelResult, parallelTime] =
            measureTime(
                [&]()
                {
                    return multiplyParallel(
                        first,
                        second,
                        workerThreads);
                });

        const bool resultsMatch =
            matricesEqual(sequentialResult, parallelResult);

        saveMat(parallelResult, "result.txt");

        logBenchmark(
            "benchmark.csv",
            matrixSize,
            workerThreads,
            sequentialTime,
            parallelTime);

        const double acceleration =
            (parallelTime > 0.0)
                ? sequentialTime / parallelTime
                : 0.0;

        std::cout << "Sequential time: "
                  << sequentialTime << " ms\n";

        std::cout << "Parallel time:   "
                  << parallelTime << " ms\n";

        std::cout << "Speed: "
                  << acceleration << '\n';

        std::cout << "C++ check: "
                  << (resultsMatch ? "OK" : "FAILED") << '\n';

        std::cout << "\nResult: result.txt\n";
        std::cout << "Benchmark: benchmark.csv\n";

        return resultsMatch ? 0 : 2;
    }
    catch (const std::exception &exception)
    {
        std::cerr << "ERROR: "
                  << exception.what() << '\n';

        return 1;
    }
}