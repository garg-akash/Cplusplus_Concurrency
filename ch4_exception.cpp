#include <iostream>
#include <future>
#include <math.h>

double sqroot(int x)
{
	std::cout << "sqrt thread : " << std::this_thread::get_id() << "\n";
	if(x < 0)
		throw std::out_of_range("x<0");
	return sqrt(x);
}

int main(int argc, char const *argv[])
{
	std::cout << "main thread : " << std::this_thread::get_id() << "\n";
	std::promise<double>* pro = new std::promise<double>;

	pro->set_value(sqroot(4));
	delete pro; // deleting the promise throws future_error
	std::future<double> fut = pro->get_future();
	
	// std::future<double> fut = std::async(sqroot,-4); // this sets the exception automatically

	std::cout << "result : " << fut.get() << "\n";
	return 0;
}