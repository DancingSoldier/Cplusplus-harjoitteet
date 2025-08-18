#include <iostream>
#include <vector>
#include <thread>
#include <numeric>  // for std::accumulate
#include <random>
#include <future>

float laske_pii(int N) {
	int n = 0;
	
	int i = 0;
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(-1, 1); //doubles from -1 to 1
	for (int i = 0; i < N; i++) {
		double random_numberX = distribution(generator);
		double random_numberY = distribution(generator);
		if (sqrt(random_numberX * random_numberX + random_numberY * random_numberY) <= 1) {
			n = n + 1;
		}
		i = i + 1;

	}
	float pii = (4 * n) / (float)N;

	printf("Piin likiarvo: %6f\n", pii);
	return pii;
}
int main() {


	std::future<float> fut1 = std::async(std::launch::async, laske_pii,100000);
	std::future<float> fut2 = std::async(std::launch::async, laske_pii,1000000);
	std::future<float> fut3 = std::async(std::launch::async, laske_pii,1555555);
	return 0;
}