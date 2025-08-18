#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <execution>

void get_numbers() {
    std::srand(unsigned(std::time(nullptr)));
    std::vector<int> v(100000);
    std::generate(v.begin(), v.end(), std::rand);
    std::sort(std::execution::par, v.begin(), v.end());
    int even_count = std::count_if(std::execution::par,
        v.begin(), v.end(),
        [](int n) {return n % 2 == 0; });

    std::cout << "Total numbers: " << v.size() << "\n";
    std::cout << "Even numbers: " << even_count << "\n";
    std::cout << "Odd numbers: " << v.size() - even_count << "\n";
}
int main() {
    get_numbers();


    return 0;
}
