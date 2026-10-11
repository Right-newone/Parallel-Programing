import std;
import matrix_core_n_files;
import matrix_ops;

namespace
{
    bool sameMat(
        const Mat &firstMatrix,
        const Mat &secondMatrix,
        double tolerance = 1e-9)
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
                    std::abs(
                        firstMatrix.at(row, column) -
                        secondMatrix.at(row, column));

                if (difference > tolerance)
                {
                    return false;
                }
            }
        }

        return true;
    }

    template <typename Function>
    std::pair<Mat, double> timed(Function &&function)
    {
        const auto start = std::chrono::steady_clock::now();

        Mat result = function();

        const auto finish = std::chrono::steady_clock::now();

        const double elapsedMs =
            std::chrono::duration<double, std::milli>(
                finish - start)
                .count();

        return {std::move(result), elapsedMs};
    }

    std::vector<std::size_t> makeThreadCounts(
        std::size_t matrixSize)
    {
        std::size_t availableThreads =
            std::thread::hardware_concurrency();

        if (availableThreads == 0)
        {
            availableThreads = 4;
        }

        const std::size_t maxThreads =
            std::min(availableThreads, matrixSize);

        std::vector<std::size_t> threadCounts{1};

        for (std::size_t count = 2; count <= maxThreads;)
        {
            threadCounts.push_back(count);

            if (count > maxThreads / 2)
            {
                break;
            }

            count *= 2;
        }

        if (threadCounts.back() != maxThreads)
        {
            threadCounts.push_back(maxThreads);
        }

        return threadCounts;
    }
}

int main(int argc, char *argv[])
{
    try
    {
        // Чистим старый CSV, чтобы каждый запуск начинался с нуля
        std::error_code ec;
        std::filesystem::remove("benchmark.csv", ec);

        const std::vector<std::size_t> sizes =
            {200, 400, 800, 1200, 1600, 2000};

        std::cout << "Parallel Programming - Lab2\n";
        std::cout << "OpenMP matrix multiplication\n\n";

        bool allResultsCorrect = true;

        for (const std::size_t size : sizes)
        {
            const std::string firstPath =
                "matrix_" + std::to_string(size) + "_a.txt";
            const std::string secondPath =
                "matrix_" + std::to_string(size) + "_b.txt";

            std::cout << "=== Matrix size: "
                      << size << " x " << size << " ===\n";

            const Mat firstMatrix = loadMat(firstPath);
            const Mat secondMatrix = loadMat(secondPath);

            if (firstMatrix.dimension() != secondMatrix.dimension())
            {
                throw std::runtime_error(
                    "Matrix dimensions must be equal.");
            }

            const auto [sequentialResult, sequentialMs] =
                timed(
                    [&]()
                    {
                        return multiplySequential(
                            firstMatrix,
                            secondMatrix);
                    });

            const auto threadCounts = makeThreadCounts(size);

            Mat lastParallelResult(size);

            for (const std::size_t threadCount : threadCounts)
            {
                const auto [parallelResult, parallelMs] =
                    timed(
                        [&]()
                        {
                            return multiplyParallel(
                                firstMatrix,
                                secondMatrix,
                                threadCount);
                        });

                const bool resultsMatch =
                    sameMat(sequentialResult, parallelResult);

                allResultsCorrect =
                    allResultsCorrect && resultsMatch;

                const double speedup =
                    parallelMs > 0.0
                        ? sequentialMs / parallelMs
                        : 0.0;

                logBenchmark(
                    "benchmark.csv",
                    size,
                    threadCount,
                    sequentialMs,
                    parallelMs);

                std::cout << "  threads=" << threadCount
                          << "  seq=" << sequentialMs << " ms"
                          << "  par=" << parallelMs << " ms"
                          << "  speedup=" << speedup
                          << "  check="
                          << (resultsMatch ? "OK" : "FAILED")
                          << '\n';

                lastParallelResult = std::move(parallelResult);
            }

            const std::string resultPath =
                "result_" + std::to_string(size) + ".txt";

            saveMat(lastParallelResult, resultPath);

            std::cout << "  saved: " << resultPath << "\n\n";
        }

        std::cout << "Benchmark CSV: benchmark.csv\n";

        return allResultsCorrect ? 0 : 2;
    }
    catch (const std::exception &exception)
    {
        std::cerr << "ERROR: "
                  << exception.what() << '\n';

        return 1;
    }
}