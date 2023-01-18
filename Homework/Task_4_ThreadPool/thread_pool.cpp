#include "thread_pool.h"

SimpleThreadPool::SimpleThreadPool(std::size_t thread_count) : m_thread_count_(
    thread_count) {
  threads_.reserve(thread_count);
  WorkOn();
}

SimpleThreadPool::~SimpleThreadPool() {
  Destroy();
}

void SimpleThreadPool::WorkOn() {
  if (stopped_.load()) {
    throw std::runtime_error("ThreadPool was destroyed");
  }

  for (size_t i = 0; i < m_thread_count_; ++i) {
    threads_.emplace_back([this]() {
      WorkerRoutine();
    });
  }
}

void SimpleThreadPool::Destroy() {
  if (stopped_.load()) return;

  stopped_.store(true);
  tasks_.Cancel();
  for (auto& thread : threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
}

void SimpleThreadPool::WorkerRoutine() {
  while (!stopped_.load()) {
    auto task = tasks_.Take();
    if (task.has_value()) {
      task.value()();
    }
  }
}
