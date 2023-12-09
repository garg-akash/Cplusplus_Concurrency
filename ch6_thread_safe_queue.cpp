#include <iostream>
#include <queue>
#include <condition_variable>

template<typename T>
class threadsafe_queue
{
	std::queue<T> q;
	std::mutex m;
	std::condition_variable con;
public:
	threadsafe_queue() {}

	void push(T new_val)
	{	
		std::lock_guard<std::mutex> lk(m);
		q.push(std::move(new_val));
		con.notify_one();
	}

	void wait_and_pop(T& val)
	{
		std::unique_lock<std::mutex> lk(m);
		con.wait(lk,[this]{return !q.empty();}); // the function won't return untill there's an element in the queue
		val = std::move(q.front());
		q.pop();
	}

	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(m);
		con.wait(lk,[this]{return !q.empty();});
		std::shared_ptr<T> val(std::make_shared<T>(std::move(q.front())));
		q.pop();
		return val;
	}

	bool try_pop(T& val)
	{
		std::lock_guard<std::mutex> lk(m);
		if(q.empty())
			return false;
		val = std::move(q.front());
		q.pop();
		return true;
	}

	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(m);
		if(q.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> val(std::make_shared<T>(std::move(q.front())));
		q.pop();
		return val;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lk(m);
		return q.empty();
	}
};

int main(int argc, char const *argv[])
{
	threadsafe_queue<int> q;
	// int val;
	// std::cout << "Can pop ? " << q.try_pop(val) << "\n";
	// q.push(5);
	// std::cout << "Can pop ? " << q.try_pop(val) << "\n";
	// std::cout << val << "\n";

	// std::shared_ptr<int> val = q.wait_and_pop();
	// std::cout << *val << "\n";

	int val;
	q.wait_and_pop(val); // blocked 
	std::cout << "Val is " << val << "\n";
	q.push(5);
	return 0;
}