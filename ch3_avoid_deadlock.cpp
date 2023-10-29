#include <iostream>
#include <mutex>

class some_big_object
{
	int a;
	std::string b;
};

void swap(some_big_object& lhs, some_big_object& rhs);

class X
{
private:
	some_big_object data;
	std::mutex m;
public:
	X(some_big_object const& d) : data(d){}
	
	friend void swap(X& lhs, X& rhs)
	{
		if(&lhs == &rhs)
			return;
		std::lock(lhs.m,rhs.m);
		std::lock_guard<std::mutex> la(lhs.m,std::adopt_lock);
		std::lock_guard<std::mutex> lb(rhs.m,std::adopt_lock);
		swap(lhs.data,rhs.data);
	}
};

int main(int argc, char const *argv[])
{
	return 0;
}