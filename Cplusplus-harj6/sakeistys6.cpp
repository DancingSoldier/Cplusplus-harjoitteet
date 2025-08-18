#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

// Flattened row-major matrix: A[r * cols + c]
using Matrix = std::vector<double>;

struct Dim { size_t rows, cols; };

Matrix make_random(size_t rows, size_t cols, unsigned seed = 42) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    Matrix M(rows * cols);
    for (auto& x : M) x = dist(rng);
    return M;
}

Matrix transpose(const Matrix& M, Dim d) {
    Matrix T(d.cols * d.rows);
    for (size_t r = 0; r < d.rows; ++r)
        for (size_t c = 0; c < d.cols; ++c)
            T[c * d.rows + r] = M[r * d.cols + c];
    return T;
}

// C = A (m x k) * B (k x n)   — sequential
void multiply_seq(const Matrix& A, Dim Ad, const Matrix& B, Dim Bd, Matrix& C) {
    const size_t m = Ad.rows, k = Ad.cols, n = Bd.cols;
    const Matrix BT = transpose(B, Bd);
    for (size_t i = 0; i < m; ++i) {
        const double* Ai = &A[i * k];
        for (size_t j = 0; j < n; ++j) {
            const double* BTj = &BT[j * k];
            double sum = 0.0;
            for (size_t t = 0; t < k; ++t) sum += Ai[t] * BTj[t];
            C[i * n + j] = sum;
        }
    }
}

// Worker for a chunk of rows [row_begin, row_end)
void multiply_chunk(const Matrix& A, Dim Ad,
    const Matrix& BT,
    size_t row_begin, size_t row_end,
    Matrix& C, size_t n)
{
    const size_t k = Ad.cols;
    for (size_t i = row_begin; i < row_end; ++i) {
        const double* Ai = &A[i * k];
        double* Ci = &C[i * n];
        for (size_t j = 0; j < n; ++j) {
            const double* BTj = &BT[j * k];
            double sum = 0.0;
            for (size_t t = 0; t < k; ++t) sum += Ai[t] * BTj[t];
            Ci[j] = sum;
        }
    }
}

// C = A * B — multithreaded with row-chunking
void multiply_mt(const Matrix& A, Dim Ad, const Matrix& B, Dim Bd,
    Matrix& C, size_t num_threads)
{
    const size_t m = Ad.rows, k = Ad.cols, n = Bd.cols;
    Matrix BT = transpose(B, Bd);

    if (num_threads == 0) num_threads = 1;
    num_threads = std::min(num_threads, m);

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    // divide rows into roughly equal chunks
    size_t chunk = (m + num_threads - 1) / num_threads; 
    for (size_t t = 0; t < num_threads; ++t) {
        size_t row_begin = t * chunk;
        if (row_begin >= m) break;
        size_t row_end = std::min(row_begin + chunk, m);
        threads.emplace_back(multiply_chunk, std::cref(A), Ad,
            std::cref(BT),
            row_begin, row_end,
            std::ref(C), n);
    }
    for (auto& th : threads) th.join();
}

bool nearly_equal(const Matrix& X, const Matrix& Y, double eps = 1e-9) {
    if (X.size() != Y.size()) return false;
    for (size_t i = 0; i < X.size(); ++i)
        if (std::fabs(X[i] - Y[i]) > eps) return false;
    return true;
}

int main() {
    // Problem size (500x500 default)
    const size_t m = 500, k = 500, n = 500;


    Matrix A = make_random(m, k, 123);
    Matrix B = make_random(k, n, 456);
    Matrix C_seq(m * n), C_mt(m * n);

    // --- Sequential ---
    auto t0 = std::chrono::high_resolution_clock::now();
    multiply_seq(A, { m, k }, B, { k, n }, C_seq);
    auto t1 = std::chrono::high_resolution_clock::now();
    auto seq_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

    // --- Multithreaded (row-chunking) ---
    size_t hw = std::max(1u, std::thread::hardware_concurrency()); // hint from CPU
    auto t2 = std::chrono::high_resolution_clock::now();
    multiply_mt(A, { m, k }, B, { k, n }, C_mt, hw);
    auto t3 = std::chrono::high_resolution_clock::now();
    auto mt_ms = std::chrono::duration<double, std::milli>(t3 - t2).count();

    // Correctness check
    bool ok = nearly_equal(C_seq, C_mt, 1e-6);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Matrix sizes: A(" << m << "x" << k << "), B(" << k << "x" << n << ")\n";
    std::cout << "Sequential:      " << seq_ms << " ms\n";
    std::cout << "Multithreaded (" << hw << " threads): " << mt_ms << " ms\n";
    std::cout << "Speedup:         " << (seq_ms / mt_ms) << "x\n";


    return 0;
}
