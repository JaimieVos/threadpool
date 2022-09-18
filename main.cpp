#include <iostream>
#include <chrono>

#include "threadpool.h"

void task(const int i)
{
	std::cout << "Started task #" << i << '\n';
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "Ended task #" << i << '\n';
}

int main()
{
	Threadpool pool(2);

	for (int i = 0; i < 10; i++)
	{
		pool.enqueue(task, i);
	}
}