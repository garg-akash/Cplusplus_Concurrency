#include <iostream>
#include <mutex>
#include <stack>
#include <memory>
#include <exception>

struct empty_stack: std::exception
{	
	const char* what() const throw();
};

template<typename T>
class threadsafe_stack
{
private:
	std::mutex m;
	mutable std::stack<T> data;
public:
	threadsafe_stack(){}
	threadsafe_stack(const threadsafe_stack& other)
	{
		std::lock_guard<std::mutex> l(other.m);
		data = other.data;
	}
	threadsafe_stack& operator=(const threadsafe_stack&) = delete;

	void push(T new_val)
	{
		std::lock_guard<std::mutex> l(m);
		data.push(new_val);
	}

	std::shared_ptr<T> pop()
	{
		std::lock_guard<std::mutex> l(m);
		if(data.empty()) throw empty_stack();
		std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
		data.pop();
		return res;
	}

	void pop(T& val)
	{
		std::lock_guard<std::mutex> l(m);
		if(data.empty()) throw empty_stack();
		val = data.top();
		data.pop();
	}
	
	bool empty() const
	{
		std::lock_guard<std::mutex> l(m);
		return data.empty();
	}
};

int main(int argc, char const *argv[])
{
	return 0;
}