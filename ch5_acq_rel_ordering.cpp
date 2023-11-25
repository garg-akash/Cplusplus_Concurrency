#include <iostream>
#include <thread>
#include <atomic>
#include <assert.h>

std::atomic<bool> x(false), y(false);
std::atomic<int> z(0);

void write_x_then_y()
{
	x.store(true,std::memory_order_relaxed); // happens-before
	y.store(true,std::memory_order_release);
}

void read_y_then_x()
{
	while(!y.load(std::memory_order_acquire)); // synchronized with store
	if(x.load(std::memory_order_relaxed))
		++z;
}

int main(int argc, char const *argv[])
{
	std::thread t1(write_x_then_y);
	std::thread t2(read_y_then_x);

	t1.join();
	t2.join();

	assert(z.load() != 0); // can never fire
	return 0;
}