#include <iostream>
#include <future>	//promise, future
#include <functional>  // std::ref

void util(std::promise<int>& p)
{
	std::this_thread::sleep_for(std::chrono::seconds(2));
	p.set_value(10);
}
int main(int argc, char const *argv[])
{
	std::promise<int> p;
	std::future<int> fut = p.get_future();
	std::thread th(util, std::ref(p));
	std::cout << "Future val : " << fut.get() << "\n"; // blocks untill a valid result is ready or throws an exception
	th.join();
	return 0;
}