#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>


void reduceFunc(int &x) {
	if (x > 100) x -= 100;
	else x = 0;
}
struct Reductor {
	void operator()(int& x) const {
		if (x > 100) x -= 100;
		else x = 0;
	}
};
int main() {
	std::vector <int> hp{ 1100, 1305, 1721, 120, 230 };
	// Funktio
	std::for_each(hp.begin(), hp.end(), reduceFunc);
	// Strukti
	std::for_each(hp.begin(), hp.end(), Reductor());
	// Lambda
	std::for_each(hp.begin(), hp.end(), [](int& x) {
		if (x > 100) x -= 100;
		else x = 0;
	});
	// Nimetty Lamda
	auto LamdaReduce = [](int& x) {
		if (x > 100) x -= 100;
		else x = 0;
		};
	std::for_each(hp.begin(), hp.end(), LamdaReduce);
	// jokaisesta hp määrästä poistetaan 400 hp yhteensä jokaisen vähennyksen jälkeen
	sort(hp.begin(), hp.end(), [](int a, int b) {
		return a > b;
		});

	for (auto v : hp) std::cout << v << " ";
		std::cout << "\n";


}
