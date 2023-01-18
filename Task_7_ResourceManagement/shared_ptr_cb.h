#ifndef TASK_7_RESOURCEMANAGMENT__SHARED_PTR_H_
#define TASK_7_RESOURCEMANAGMENT__SHARED_PTR_H_

#include <iostream>

namespace sp {

template<typename T>
struct ControlBlock {
  size_t shared_count = 0;
  size_t weak_count = 0;
  T object = nullptr;

//  template<typename... Args>
//  explicit ControlBlock(size_t count, Args&& ... args)
//      : shared_count(count), object(T(std::forward<Args>(args)...)) {
//  }

};

template<typename T>
class shared_ptr {
 public:
  shared_ptr() = default;
  explicit shared_ptr(T* ptr);
  shared_ptr(const shared_ptr<T>& shared);
  shared_ptr(shared_ptr<T>&& shared) noexcept;

  shared_ptr<T>& operator=(shared_ptr<T> shared);
  shared_ptr<T>& operator=(shared_ptr<T>&& shared) noexcept;

  explicit operator bool() const noexcept;

  T* get() const noexcept;

  size_t use_count() const noexcept;

  ~shared_ptr();

 private:
  explicit shared_ptr(ControlBlock<T>* control_block);

  template<typename U, typename... Args>
  friend shared_ptr<U> make_shared(Args&& ... args);

  template<typename U>
  class weak_ptr;

  T* ptr_ = nullptr;
  ControlBlock<T>* control_block_ = nullptr;
};

template<typename T>
shared_ptr<T>::shared_ptr(T* ptr) :
    control_block_(new ControlBlock<T>(1, 0, ptr_)), ptr_(ptr) {}

template<typename T>
shared_ptr<T>::shared_ptr(const shared_ptr<T>& shared) :
    control_block_(shared.control_block_), ptr_(shared.ptr_) {
  ++control_block_->count;
}

template<typename T>
shared_ptr<T>::shared_ptr(shared_ptr<T>&& shared) noexcept :
    control_block_(shared.control_block_), ptr_(shared.ptr_) {
  shared.control_block_ = nullptr;
  shared.ptr_ = nullptr;
}

template<typename T>
shared_ptr<T>::shared_ptr(ControlBlock<T>* control_block)
    : control_block_(control_block), ptr_(&control_block->object) {

}

template<typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<T> shared) {
  std::swap(shared.control_block_, control_block_);
  std::swap(shared.ptr_, ptr_);

  return *this;
}

template<typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<T>&& shared) noexcept {
  control_block_ = shared.control_block_;
  ptr_ = shared.ptr_;

  shared.control_block_ = nullptr;
  shared.ptr_ = nullptr;

  return *this;
}

template<typename T>
shared_ptr<T>::operator bool() const noexcept {
  return ptr_ != nullptr;
}

template<typename T>
T* shared_ptr<T>::get() const noexcept {
  return ptr_;
}

template<typename T>
size_t shared_ptr<T>::use_count() const noexcept {
  if (control_block_ && control_block_->count) {
    return *control_block_->count;
  } else {
    return 0;
  }
}

template<typename T>
shared_ptr<T>::~shared_ptr() {
  if (!control_block_) return;
  --control_block_->shared_count;
  if (control_block_->shared_count == 0) {
    delete ptr_;
    if (control_block_->weak_count == 0) {
      delete control_block_;
    }
  }

}

template<typename T, typename... Args>
shared_ptr<T> make_shared(Args&& ... args) {
  auto p = new ControlBlock<T>{1, 0, T(std::forward<Args>(args)...)};
  return shared_ptr<T>(p);
}

template<typename T>
class weak_ptr {
 public:
  weak_ptr(const shared_ptr<T>& shared) :
      control_block_(shared.control_block_) {
    ++control_block_->weak_count;
  }

  bool expired() const {
    if (control_block_ && control_block_->shared_count != 0) {
      return false;
    } else {
      return true;
    }
  }

  shared_ptr<T> lock() const {
    return shared_ptr<T>(control_block_);
  }

  ~weak_ptr() {
    if (!control_block_) return;
    --control_block_->weak_count;
    if (control_block_->weak_count == 0 && control_block_->shared_count == 0) {
      delete control_block_;
    }
  }

 private:
  ControlBlock<T>* control_block_ = nullptr;
};

} // namespace

#endif //TASK_7_RESOURCEMANAGMENT__SHARED_PTR_H_
