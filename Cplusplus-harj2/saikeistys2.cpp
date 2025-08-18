#include <iostream>       // std::cout
#include <thread>         // std::thread



void print()
{

	std::cout << "Thread used is: " << " (id=" << std::this_thread::get_id() << ")\n";

}
int main()
{
	std::thread first(print);     // spawn new thread that calls foo()
	std::thread second(print);  // spawn new thread that calls bar(0)
	std::thread third(print);
	std::thread fourth(print);

	// synchronize threads:
	first.join();                // pauses until first finishes
	second.join();               // pauses until second finishes
	third.join();
	fourth.join();

	return 0;
}