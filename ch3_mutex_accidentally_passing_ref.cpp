#include <iostream>
#include <mutex>

using namespace std;

class some_data
{
	int x;
	string str;
public:
	void do_something()
	{
		x++;
	}
};

class data_wrapper
{
	some_data data;
	std::mutex m;
public:
	template<typename Function>
	void process_data(Function func)
	{
		std::lock_guard<std::mutex> l(m);
		func(data);
	}
};

some_data* unprotected;
void malacious_func(some_data& protected_data)
{
	unprotected = &protected_data;
}

data_wrapper x;
void foo()
{
	x.process_data(malacious_func);
	unprotected->do_something();
}

int main(int argc, char const *argv[])
{
	foo();
	return 0;
}