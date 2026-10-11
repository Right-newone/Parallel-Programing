export module matrix_ops;

import std;
import matrix_core_n_files;

export Mat multiplySequential(
    const Mat& leftMatrix,
    const Mat& rightMatrix)
{
    const std::size_t size = leftMatrix.dimension();

    if (size == 0 || size != rightMatrix.dimension())
    {
        throw std::invalid_argument(
            "Matrix dimensions must be equal and positive.");
    }

    Mat product(size);

    for (std::size_t row = 0; row < size; ++row)
    {
        for (std::size_t column = 0; column < size; ++column)
        {
            double value = 0.0;

            for (std::size_t index = 0; index < size; ++index)
            {
                value += leftMatrix.at(row, index) *
                         rightMatrix.at(index, column);
            }

            product.at(row, column) = value;
        }
    }

    return product;
}

export Mat multiplyParallel(
    const Mat& leftMatrix,
    const Mat& rightMatrix,
    std::size_t requestedThreads)
{
    const std::size_t size = leftMatrix.dimension();

    if (size == 0 || size != rightMatrix.dimension())
    {
        throw std::invalid_argument(
            "Matrix dimensions must be equal and positive.");
    }

    if (size >
        static_cast<std::size_t>(
            std::numeric_limits<int>::max()))
    {
        throw std::invalid_argument(
            "Matrix is too large for the OpenMP loop index.");
    }

    if (requestedThreads == 0)
    {
        requestedThreads = 1;
    }

    requestedThreads = std::min(requestedThreads, size);

    const int rowCount = static_cast<int>(size);
    const int threadCount = static_cast<int>(requestedThreads);

    Mat product(size);

    #pragma omp parallel for num_threads(threadCount) schedule(static)
    for (int row = 0; row < rowCount; ++row)
    {
        const std::size_t currentRow =
            static_cast<std::size_t>(row);

        for (std::size_t column = 0; column < size; ++column)
        {
            double value = 0.0;

            for (std::size_t index = 0; index < size; ++index)
            {
                value += leftMatrix.at(currentRow, index) *
                         rightMatrix.at(index, column);
            }

            product.at(currentRow, column) = value;
        }
    }

    return product;
}