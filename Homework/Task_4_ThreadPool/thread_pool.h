#ifndef TASK_4_THREADPOOL__THREAD_POOL_H_
#define TASK_4_THREADPOOL__THREAD_POOL_H_

#include <vector>
#include <thread>
#include <future>

#include "blocking_queue.h"

/* ------------------------
 * --- Function Wrapper ---
 * ------------------------*/
class FunctionWrapper {
 public:
  FunctionWrapper() = default;

  template<typename Fnc_T>
  FunctionWrapper(Fnc_T&& f):
      impl_(new ImplType<Fnc_T>(std::move(f))) {}

  FunctionWrapper(FunctionWrapper&& other) :
      impl_(std::move(other.impl_)) {}

  FunctionWrapper& operator=(FunctionWrapper&& other) {
    impl_ = std::move(other.impl_);
    return *this;
  }

  FunctionWrapper(const FunctionWrapper&) = delete;
  FunctionWrapper(FunctionWrapper&) = delete;
  FunctionWrapper& operator=(const FunctionWrapper&) = delete;

  void operator()() {
    impl_->call();
  }

 private:
  struct ImplBase {
    virtual void call() = 0;
    virtual ~ImplBase() = default;
  };

  template<typename Fnc_T>
  struct ImplType : ImplBase {
    Fnc_T func_;
    ImplType(Fnc_T&& f_) : func_(std::move(f_)) {}
    void call() { func_(); }
  };

  std::unique_ptr<ImplBase> impl_;
};

/* -------------------
 * --- Thread Pool ---
 * -------------------*/
class SimpleThreadPool {
 public:
  explicit SimpleThreadPool(std::size_t threadCount);

  ~SimpleThreadPool();

  SimpleThreadPool(const SimpleThreadPool&) = delete;
  SimpleThreadPool& operator=(const SimpleThreadPool&) = delete;

  template<typename Fnc_T>
  auto Post(Fnc_T task) -> std::future<decltype(task())>;

  void Destroy();

 private:
  void WorkOn();
  void WorkerRoutine();

  size_t m_thread_count_;
  std::vector<std::thread> threads_;
  BlockingQueue<FunctionWrapper> tasks_;
  std::atomic<bool> stopped_{false};
};

template<typename Fnc_T>
auto SimpleThreadPool::Post(Fnc_T task) -> std::future<decltype(task())> {
  using result_type = typename std::result_of<Fnc_T()>::type;

  std::packaged_task<result_type()> packaged_task(std::move(task));
  std::future<result_type> future = packaged_task.get_future();
  tasks_.Put(std::move(packaged_task));

  return future;
}

#endif //TASK_4_THREADPOOL__THREAD_POOL_H_
