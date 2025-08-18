#include <iostream>
#include <vector>
#include <thread>
#include <numeric>  // for std::accumulate


void partialSum(const std::vector<int>& vec, size_t start, size_t end, long long& result) {
    result = std::accumulate(vec.begin() + start, vec.begin() + end, 0LL);
}

int main() {
    const size_t VECTOR_SIZE = 1'000'000;  // Large vector
    const size_t NUM_THREADS = 4;          // Number of threads

    // Generate a large vector with values 1 to VECTOR_SIZE
    std::vector<int> data(VECTOR_SIZE);
    std::iota(data.begin(), data.end(), 1);

    // Prepare threads
    std::vector<std::thread> threads;
    std::vector<long long> partialSums(NUM_THREADS, 0);

    size_t chunkSize = VECTOR_SIZE / NUM_THREADS;

    // threads created
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == NUM_THREADS - 1) ? VECTOR_SIZE : (i + 1) * chunkSize;
        threads.emplace_back(partialSum, std::cref(data), start, end, std::ref(partialSums[i]));
    }

    // joined
    for (auto& t : threads) {
        t.join();
    }

    // Ccmbine partial sums
    long long totalSum = std::accumulate(partialSums.begin(), partialSums.end(), 0LL);

    std::cout << "Total Sum: " << totalSum << std::endl;

    return 0;
}
