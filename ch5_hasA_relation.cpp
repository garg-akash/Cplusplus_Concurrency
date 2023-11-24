#include <iostream>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

std::atomic<bool> data_flag(false);
std::vector<int> data;

void reader()
{
  while(!data_flag)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  std::cout << "Data is : " << data[0];
}

void writer()
{
  data.push_back(9);
  data_flag = true;
}

int main(int argc, char const *argv[])
{
  std::thread t1(reader);
  std::thread t2(writer);
  t1.join();
  t2.join();
  return 0;
}
