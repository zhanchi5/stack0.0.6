#include "stack.h"
#include <thread>
#include <unistd.h>

template<typename T>
void producer(stack<int> &stack_)
{
	for (;;)
	{
		try
		{
			stack_.push(std::rand() % 100);
		}
		catch (std::bad_alloc)
		{
			std::this_thread::sleep_for(std::chrono::seconds(std::rand() % (3) + 1));
		}
		std::this_thread::sleep_for(std::chrono::seconds(std::rand() % (3) + 1));
	}
}
template<typename T>
void consumer(stack<int> &stack_)
{
	for (;;)
	{
		try
		{
			std::shared_ptr<int> ptr = stack_.try_pop();
			if (ptr == nullptr)
			{
				stack_.wait_and_pop();
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(std::rand() % (3) + 2));
	}
}
int main(){
    stack<int> st1;
    std::mutex mt1;
    std::thread t1(consumer<int>, std::ref(st1), std::ref(mt1));
    std::thread t2(producer<int>, std::ref(st1), std::ref(mt1));
    std::thread t3(consumer<int>, std::ref(st1), std::ref(mt1));
    std::thread t4(producer<int>, std::ref(st1), std::ref(mt1));
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    return 0;
}
