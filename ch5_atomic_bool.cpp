#include <iostream>
#include <atomic>

int main(int argc, char const *argv[])
{
	/*using atomic flag*/
	// atomic flag is guranteed to be lock free
	std::atomic_flag f = ATOMIC_FLAG_INIT; // must be initilized (cannot be copied or assigned)

	bool x = f.test_and_set(); // set and query the previous value
	std::cout << x << "\n";
	
	x = f.test_and_set(); // already set
	std::cout << x << "\n";
	
	f.clear(std::memory_order_release); // clear the flag
	x = f.test_and_set();
	std::cout << x << "\n";

	/*using atomic bool*/
	std::atomic<bool> b ;
	
	bool y = b.load();
	std::cout << y << "\n";
	
	b.store(true);
	std::cout << "is lock free? : " << b.is_lock_free() << "\n";
	y = b.exchange(false);
	std::cout << y << "\n";
	return 0;
}