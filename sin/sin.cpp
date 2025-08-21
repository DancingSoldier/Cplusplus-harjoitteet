#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>

#define DEG_TO_RAD(A) ((A) / 57.296)
#define ITERATIONS 1000000
#define ANGLES 360

int main() {
    clock_t t1, t2;
    double val;

    std::vector<double> sintable(ANGLES);
    for (int a = 0; a < ANGLES; ++a)
        sintable[a] = std::sin(DEG_TO_RAD(a));

    // Benchmark
    t1 = clock();
    for (int i = 0; i < ITERATIONS; ++i) {
        for (int a = 0; a < ANGLES; ++a) {
            val = sintable[a];  // Käytä esilaskettua arvoa
        }
    }
    t2 = clock();

    std::cout << "Optimized: " << (t2 - t1) / static_cast<float>(CLOCKS_PER_SEC) << "s\n";

    return 0;
}
