#include <iostream>
#include <vector>
#include <numeric>   // for std::accumulate
#include <future>    // for std::async, std::future

// Function to compute partial sum
long long partialSum(const std::vector<int>& vec, size_t start, size_t end) {
    return std::accumulate(vec.begin() + start, vec.begin() + end, 0LL);
}

int main() {
    const size_t VECTOR_SIZE = 1'000'000;  // Large vector
    const size_t NUM_THREADS = 4;          // Number of tasks

    std::vector<int> data(VECTOR_SIZE);
    std::iota(data.begin(), data.end(), 1);

    // futures
    std::vector<std::future<long long>> futures;
    size_t chunkSize = VECTOR_SIZE / NUM_THREADS;


    for (size_t i = 0; i < NUM_THREADS; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == NUM_THREADS - 1) ? VECTOR_SIZE : (i + 1) * chunkSize;

        futures.push_back(std::async(std::launch::async, partialSum, std::cref(data), start, end));
    }
    long long totalSum = 0;
    for (auto& f : futures) {
        totalSum += f.get();   // wait for result and add
    }

    std::cout << "Total Sum: " << totalSum << std::endl;

    return 0;
}
