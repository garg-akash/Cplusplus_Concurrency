#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

std::atomic<int> x(0), y(0), z(0);
std::atomic<bool> go(false);

int const loop_count = 10;

struct read_val
{
	int x,y,z;
};

read_val val1[loop_count];
read_val val2[loop_count];
read_val val3[loop_count];
read_val val4[loop_count];
read_val val5[loop_count];

void increment(std::atomic<int>* val_to_inc, read_val* val)
{
	while(!go)
	{
		std::this_thread::yield();
	}
	for (int i = 0; i < loop_count; ++i)
	{
		val[i].x = x.load(std::memory_order_relaxed);
		val[i].y = y.load(std::memory_order_relaxed);
		val[i].z = z.load(std::memory_order_relaxed);
		val_to_inc->store(i+1,std::memory_order_relaxed);
		std::this_thread::yield();
	}
}

void read_values(read_val* val)
{
	while(!go)
	{
		std::this_thread::yield();
	}
	for (int i = 0; i < loop_count; ++i)
	{
		val[i].x = x.load(std::memory_order_relaxed);
		val[i].y = y.load(std::memory_order_relaxed);
		val[i].z = z.load(std::memory_order_relaxed);
		std::this_thread::yield();
	}
}

void print_values(read_val* val)
{
	for (int i = 0; i < loop_count; ++i)
	{
		if(i)
			std::cout << ",";
		std::cout << "(" << val[i].x << "," << val[i].y << "," << val[i].z << ")"; 
	}
	std::cout << "\n";
}

int main(int argc, char const *argv[])
{
	std::thread t1(increment,&x,val1);
	std::thread t2(increment,&y,val2);
	std::thread t3(increment,&z,val3);
	std::thread t4(read_values,val4);
	std::thread t5(read_values,val5);

	go = true;

	t5.join();
	t4.join();
	t3.join();
	t2.join();
	t1.join();

	print_values(val1);
	print_values(val2);
	print_values(val3);
	print_values(val4);
	print_values(val5);
	return 0;
}