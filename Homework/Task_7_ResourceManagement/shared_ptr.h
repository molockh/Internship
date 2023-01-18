#ifndef TASK_7_RESOURCEMANAGMENT__SHARED_PTR_SIMPLE_H_
#define TASK_7_RESOURCEMANAGMENT__SHARED_PTR_SIMPLE_H_

#include <iostream>

namespace sp {

/*-----------------
 ----shared_ptr----
 ------------------*/
template<typename T>
class shared_ptr {
 public:
  shared_ptr() = default;
  explicit shared_ptr(T* ptr);
  shared_ptr(const shared_ptr<T>& shared);
  shared_ptr(shared_ptr<T>&& shared) noexcept;

  shared_ptr<T>& operator=(shared_ptr<T> shared);
  shared_ptr<T>& operator=(shared_ptr<T>&& shared) noexcept;

  T& operator*() const noexcept;
  T* operator->() const noexcept;

  explicit operator bool() const noexcept;

  T* get() const noexcept;

  size_t use_count() const noexcept;
  bool unique() const noexcept;

  ~shared_ptr();

 private:
  shared_ptr(T* ptr, size_t* shared_count, size_t* weak_count);

  template<typename U>
  friend
  class weak_ptr;

  T* ptr_ = nullptr;
  size_t* shared_count_ = nullptr;
  size_t* weak_count_ = nullptr;
};

template<typename T>
shared_ptr<T>::shared_ptr(T* ptr) :
    ptr_(ptr), shared_count_(new size_t(1)), weak_count_(new size_t(0)) {}

template<typename T>
shared_ptr<T>::shared_ptr(const shared_ptr<T>& shared) :
    ptr_(shared.ptr_),
    shared_count_(shared.shared_count_),
    weak_count_(shared.weak_count_) {
  if (shared_count_) {
    ++*shared_count_;
  }
}

template<typename T>
shared_ptr<T>::shared_ptr(shared_ptr<T>&& shared) noexcept :
    ptr_(shared.ptr_),
    shared_count_(shared.shared_count_),
    weak_count_(shared.weak_count_) {
  shared.ptr_ = nullptr;
  shared.shared_count_ = nullptr;
  shared.weak_count_ = nullptr;
}

template<typename T>
shared_ptr<T>::shared_ptr(T* ptr, size_t* shared_count, size_t* weak_count) :
    ptr_(ptr), shared_count_(shared_count), weak_count_(weak_count) {}

template<typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<T> shared) {
  std::swap(shared.ptr_, ptr_);
  std::swap(shared.shared_count_, shared_count_);
  std::swap(shared.weak_count_, weak_count_);

  return *this;
}

template<typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<T>&& shared) noexcept {
  if (*shared_count_ != 0) {
    --*shared_count_;
    if (*shared_count_ == 0) {
      delete shared_count_;
      delete weak_count_;
    }
  }

  ptr_ = shared.ptr_;
  shared_count_ = shared.shared_count_;
  weak_count_ = shared.shared_count_;

  shared.ptr_ = nullptr;
  shared.shared_count_ = nullptr;
  shared.weak_count_ = nullptr;

  return *this;
}

template<typename T>
T& shared_ptr<T>::operator*() const noexcept {
  return *ptr_;
}

template<typename T>
T* shared_ptr<T>::operator->() const noexcept {
  return ptr_;
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
  if (shared_count_) {
    return *shared_count_;
  }
  return 0;
}

template<typename T>
bool shared_ptr<T>::unique() const noexcept {
  return use_count() == 1;
}

template<typename T>
shared_ptr<T>::~shared_ptr() {
  if (!shared_count_) return;

  --*shared_count_;
  if (*shared_count_ > 0) {
    return;
  }

  delete ptr_;

  if (*weak_count_ == 0) {
    delete shared_count_;
    delete weak_count_;
  }
}

template<class T, class U>
bool operator==(const std::shared_ptr<T>& lhs,
                const std::shared_ptr<U>& rhs) noexcept {
  return lhs.get() == rhs.get();
}

template<class T, class U>
bool operator!=(const std::shared_ptr<T>& lhs,
                const std::shared_ptr<U>& rhs) noexcept {
  return !(lhs == rhs);
}

template<class T>
bool operator==(const std::shared_ptr<T>& lhs, std::nullptr_t) noexcept {
  return !lhs;
}

template<class T>
bool operator==(std::nullptr_t, const std::shared_ptr<T>& rhs) noexcept {
  return !rhs;
}

template<class T>
bool operator!=(const std::shared_ptr<T>& lhs, std::nullptr_t) noexcept {
  return static_cast<bool>(lhs);
}

template<class T>
bool operator!=(std::nullptr_t, const std::shared_ptr<T>& rhs) noexcept {
  return static_cast<bool>(rhs);
}

// ----------------------------------------------------------------------

/*-----------------
 -----weak_ptr-----
 ------------------*/
template<typename T>
class weak_ptr {
 public:
  weak_ptr(const shared_ptr<T>& shared);

  size_t use_count() const noexcept;
  bool expired() const noexcept;
  shared_ptr<T> lock() const noexcept;

  ~weak_ptr();

 private:
  T* ptr_ = nullptr;
  size_t* shared_count_ = nullptr;
  size_t* weak_count_ = nullptr;
};

template<typename T>
weak_ptr<T>::weak_ptr(const shared_ptr<T>& shared) :
    ptr_(shared.ptr_),
    shared_count_(shared.shared_count_),
    weak_count_(shared.weak_count_) {
  if (weak_count_) {
    ++*weak_count_;
  }
}

template<typename T>
size_t weak_ptr<T>::use_count() const noexcept {
  if (shared_count_) {
    return *shared_count_;
  }
  return 0;
}

template<typename T>
bool weak_ptr<T>::expired() const noexcept {
  return *shared_count_ == 0;
}

template<typename T>
shared_ptr<T> weak_ptr<T>::lock() const noexcept {
  return shared_ptr<T>(ptr_, shared_count_, weak_count_);
}

template<typename T>
weak_ptr<T>::~weak_ptr() {
  if (!weak_count_) return;

  --*weak_count_;
  if (*weak_count_ == 0 && *shared_count_ == 0) {
    delete weak_count_;
    delete shared_count_;
  }
}

} // namespace

#endif //TASK_7_RESOURCEMANAGMENT__SHARED_PTR_SIMPLE_H_
