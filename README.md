# Threadpool
This repository contains a simple implementation of a threadpool. It is a header-only library, so you can just include the header file in your project and use it. The threadpool is implemented using C++17 features, so you need a compiler that supports C++17.
Feel free to use this library in your projects. If you have any questions or suggestions, feel free to open an issue or contact me.

## Usage
```c++
// Create a threadpool with a certain amount of threads
Threadpool pool(2);

// Add a task to the threadpool queue
pool.enqueue([]() { std::cout << "Hello, World!"; });
```