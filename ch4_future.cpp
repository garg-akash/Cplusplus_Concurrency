#include <iostream>
#include <future>

int foo(int i)
{
	if(i == 0)
		return 1;
	return i*foo(i-1);
}

void do_something_else()
{
	std::cout << "Inside something else\n";
}

int main(int argc, char const *argv[])
{
	std::future<int> fut = std::async(foo,5);
	std::cout << "Future val : " << fut.get() << "\n"; // blocks untill a valid result is ready or throws an exception
	do_something_else();
	return 0;
}