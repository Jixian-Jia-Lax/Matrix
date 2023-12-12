#include <iostream>
#include <chrono>
#include <vector>
#include <thread>

const int MAX_SIZE = 16;
typedef float Matrix[MAX_SIZE][MAX_SIZE];

void transposeMatrix(const Matrix& input, Matrix& output, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            output[j][i] = input[i][j];
        }
    }
}

void matrixMultiplyRecursive(Matrix &c, const Matrix &a, const Matrix &b, int size, int cRow, int cCol, int aRow, int aCol, int bRow, int bCol) {
    if (size == 1) {
        c[cRow][cCol] += a[aRow][aCol] * b[bRow][bCol];
    } else {
        int newSize = size / 2;
        std::vector<std::thread> threads;

        // Threaded recursive calls for computing sub-matrices
        threads.emplace_back(matrixMultiplyRecursive, std::ref(c), std::ref(a), std::ref(b), newSize, cRow, cCol, aRow, aCol, bRow, bCol);
        threads.emplace_back(matrixMultiplyRecursive, std::ref(c), std::ref(a), std::ref(b), newSize, cRow, cCol, aRow, aCol + newSize, bRow, bCol + newSize);
        threads.emplace_back(matrixMultiplyRecursive, std::ref(c), std::ref(a), std::ref(b), newSize, cRow + newSize, cCol, aRow + newSize, aCol, bRow, bCol);
        threads.emplace_back(matrixMultiplyRecursive, std::ref(c), std::ref(a), std::ref(b), newSize, cRow + newSize, cCol, aRow + newSize, aCol + newSize, bRow, bCol + newSize);
        threads.emplace_back(matrixMultiplyRecursive, std::ref(c), std::ref(a), std::ref(b), newSize, cRow, cCol + newSize, aRow, aCol, bRow + newSize, bCol);
        threads.emplace_back(matrixMultiplyRecursive, std::ref(c), std::ref(a), std::ref(b), newSize, cRow, cCol + newSize, aRow, aCol + newSize, bRow + newSize, bCol);
        threads.emplace_back(matrixMultiplyRecursive, std::ref(c), std::ref(a), std::ref(b), newSize, cRow + newSize, cCol + newSize, aRow + newSize, aCol, bRow + newSize, bCol);
        threads.emplace_back(matrixMultiplyRecursive, std::ref(c), std::ref(a), std::ref(b), newSize, cRow + newSize, cCol + newSize, aRow + newSize, aCol + newSize, bRow + newSize, bCol);

        for (auto &thread : threads) {
            thread.join();
        }
    }
}

int main() {
    int n = 16; // Matrix size (should be <= MAX_SIZE and even for this implementation)
    Matrix mat1 = {};  // Initialize with 0
    Matrix mat2 = {}; // Initialize with 0
    Matrix mat2Transposed = {}; // Initialize with 0
    Matrix result = {}; // Initialize with 0

    // Filling mat1 and mat2 for demonstration
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            mat1[i][j] = 1.1; // Example values
            mat2[i][j] = 2.2; // Example values
        }
    }

    transposeMatrix(mat2, mat2Transposed, n);

    auto start = std::chrono::high_resolution_clock::now();

    matrixMultiplyRecursive(result, mat1, mat2Transposed, n, 0, 0, 0, 0, 0, 0);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken: " << duration.count() << " milliseconds" << std::endl;

    // Print the result
//    for (int i = 0; i < n; ++i) {
//        for (int j = 0; j < n; ++j) {
//            std::cout << result[i][j] << ' ';
//        }
//        std::cout << '\n';
//    }

    return 0;
}
