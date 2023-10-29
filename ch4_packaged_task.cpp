#include <iostream>
#include <mutex>
#include <future>
#include <deque>
#include <functional>  // std::bind

std::mutex m;
std::deque<std::packaged_task<int()>> tasks;

int foo(int i)
{
	if(i == 0)
		return 1;
	return i*foo(i-1);
}

// bool shutdown()
// {
// 	while(10)
// 	{

// 	}
// }

void execute_task()
{
	while(true)
	{
		std::packaged_task<int()> task;
		{
			std::lock_guard<std::mutex> l(m);
			if(tasks.empty())
			{
				continue;
			}
			task = std::move(tasks.front());
			tasks.pop_front();
		}
		task();
	}
}

int main(int argc, char const *argv[])
{
	std::thread th(execute_task);
	std::packaged_task<int()> task(std::bind(foo,5));
	std::future<int> fut = task.get_future();
	{
		std::lock_guard<std::mutex> l(m);
		tasks.push_back(std::move(task));
	}
	std::cout << "Future val : " << fut.get() << "\n"; // blocks untill a valid result is ready or throws an exception
	th.join();
	return 0;
}