#include <iostream>
#include <thread>
#include <atomic>
#include <assert.h>


std::atomic<int> data[2];
std::atomic<bool> sync1(false);
std::atomic<bool> sync2(false);

void thread1()
{
	data[0].store(10,std::memory_order_relaxed);
	data[1].store(20,std::memory_order_relaxed);
	sync1.store(true,std::memory_order_release);
}

void thread2()
{
	while(!sync1.load(std::memory_order_acquire));
	sync2.store(true,std::memory_order_release);
}

void thread3()
{
	while(!sync2.load(std::memory_order_acquire));
	assert(data[0].load(std::memory_order_relaxed) == 10);
	assert(data[1].load(std::memory_order_relaxed) == 20);
}

int main(int argc, char const *argv[])
{
	std::thread t3(thread3);
	std::thread t1(thread1);
	std::thread t2(thread2);t1.join();
	t2.join();
	t3.join();
	std::cout << "finished\n";
	return 0;
}