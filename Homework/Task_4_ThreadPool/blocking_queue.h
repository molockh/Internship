#ifndef TASK_4_THREADPOOL__BLOCKING_QUEUE_H_
#define TASK_4_THREADPOOL__BLOCKING_QUEUE_H_

#include <mutex>
#include <deque>
#include <optional>

template<typename T>
class BlockingQueue {
 public:
  bool Put(T value);

  std::optional<T> Take();

  void Close();
  void Cancel();

 private:
  void CloseImpl(bool clear);
  T TakeLocked();

  std::deque<T> queue_;
  std::mutex mutex_;
  std::condition_variable not_empty_;
  std::atomic<bool> opened_{true};
};

template<typename T>
bool BlockingQueue<T>::Put(T value) {
  std::lock_guard guard(mutex_);
  if (opened_.load()) {
    queue_.push_back(std::move(value));
    not_empty_.notify_one();
    return true;
  }
  return false;
}

template<typename T>
std::optional<T> BlockingQueue<T>::Take() {
  std::unique_lock lock(mutex_);

  while (queue_.empty()) {
    if (!opened_.load()) {
      return std::nullopt;
    }

    not_empty_.wait(lock);
  }
  return TakeLocked();
}

template<typename T>
void BlockingQueue<T>::Close() {
  CloseImpl(false);
}

template<typename T>
void BlockingQueue<T>::Cancel() {
  CloseImpl(true);
}

template<typename T>
void BlockingQueue<T>::CloseImpl(bool clear) {
  std::lock_guard guard(mutex_);
  opened_.store(false);
  not_empty_.notify_all();
  if (clear) {
    queue_.clear();
  }
}

template<typename T>
T BlockingQueue<T>::TakeLocked() {
  T front(std::move(queue_.front()));
  queue_.pop_front();
  return front;
}

#endif //TASK_4_THREADPOOL__BLOCKING_QUEUE_H_
