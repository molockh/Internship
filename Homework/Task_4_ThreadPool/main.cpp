#include <iostream>
#include <vector>
#include <future>

#include "thread_pool.h"

int IntFunc(int x) {
  // Do some work
  std::this_thread::sleep_for(std::chrono::seconds(x % 2 + 1));
  std::cout << "IntFunc successfully finished work on " << x << std::endl;
  return x * x;
}

void VoidFunc() {
  // Do some work
  std::this_thread::sleep_for(std::chrono::seconds(3));
  std::cout << "VoidFunc successfully finished work" << std::endl;
}

int main() {

  {
    SimpleThreadPool pool(5);
    int futures_count = 8;
    std::vector<std::future<int>> futures(futures_count);

    for (int i = 0; i < futures_count; ++i) {
      std::cout << "Task " << i << " posted to the ThreadPool" << std::endl;
      futures[i] = pool.Post([i]() {
        return IntFunc(i);
      });
    }

    for (int i = 0; i < futures_count; i++) {
      int result = futures[i].get();
      std::cout << "Result of task " << i << ": " << result << std::endl;
    }

    pool.Destroy();
  }

  std::cout << "\n--------------------------------------\n" << std::endl;

  {
    SimpleThreadPool pool(5);
    int futures_count = 10;
    std::vector<std::future<void>> futures(futures_count);

    for (int i = 0; i < futures_count; ++i) {
      std::cout << "Task " << i << " posted to the ThreadPool" << std::endl;
      futures[i] = pool.Post(VoidFunc);
    }

    for (int i = 0; i < futures_count; i++) {
      futures[i].get();
    }

    pool.Destroy();
  }

  return EXIT_SUCCESS;
}