#ifndef TASK_2_CODEREVIEW__MYVECTOR_H_
#define TASK_2_CODEREVIEW__MYVECTOR_H_

#include <vector>
#include <utility>
#include <string>
#include <algorithm>

/*
 * MyVector stores a collection of objects with their names.
 *
 * For each object T, MyVector stores T`s name as std::string.
 * Several objects can have similar name.
 * operator[](const std::string& name) should return the first object
 * with the given name.
 *
 * Your task is to find as many mistakes and drawbacks in this code
 * (according to the presentation) as you can.
 * Annotate these mistakes with comments.
 *
 * Once you have found all the mistakes, rewrite the code
 * so it would not change its original purpose
 * and it would contain no mistakes.
 * Try to make the code more efficient without premature optimization.
 *
 * You can change MyVector interface completely, but there are several rules:
 * 1) you should correctly and fully implement copy-on-write idiom.
 * 2) std::pair<const T&, const std::string&> operator[](int index) const must take constant time at worst.
 * 3) const T& operator[](const std::string& name) const should be present.
 * 4) both operator[] should have non-const version.
 * 5) your implementation should provide all the member types of std::vector.
 * 6) your implementation should provide the following functions:
 *    1) begin(), cbegin(), end(), cend()
 *    2) empty(), size()
 *    3) reserve(), clear()
 */

template<typename T>
class MyVector {
 public:
  MyVector();
  MyVector(const MyVector<T>& vector) = default;
  ~MyVector() = default;

  MyVector& operator=(const MyVector<T>& vector) = default;

  void push_back(const T& obj, const std::string& name);

  const T& operator[](const std::string& name) const;
  T& operator[](const std::string& name);

  std::pair<T, std::string> operator[](size_t index) const;
  std::pair<T&, std::string&> operator[](size_t index);

  std::pair<T, std::string> at(size_t index) const;
  std::pair<T&, std::string&> at(size_t index);

  typename std::vector<std::pair<T, std::string>>::iterator begin() noexcept;
  typename std::vector<std::pair<T, std::string>>::iterator end() noexcept;
  typename std::vector<std::pair<T,
                                 std::string>>::const_iterator cbegin() const noexcept;
  typename std::vector<std::pair<T,
                                 std::string>>::const_iterator cend() const noexcept;

  bool empty() const noexcept;
  size_t size() const noexcept;
  size_t capacity() const noexcept;
  void reserve(size_t size);
  void clear() noexcept;

 private:
  using vector_ptr = std::vector<std::pair<T, std::string>>;
  std::shared_ptr<vector_ptr> data_;

  void DeepCopy();
};

template<typename T>
MyVector<T>::MyVector() : data_(std::make_shared<vector_ptr>()) {}

template<typename T>
void MyVector<T>::push_back(const T& obj, const std::string& name) {
  DeepCopy();
  data_->push_back({obj, name});
}

template<typename T>
const T& MyVector<T>::operator[](const std::string& name) const {
  auto it = std::find_if(data_->begin(),
                         data_->end(),
                         [name](const std::pair<T, std::string>& value) {
                           return value.second == name;
                         });

  if (it == data_->end())
    throw std::invalid_argument(name + " is not found in the MyVector");
  size_t index = it - data_->begin();

  return data_->at(index).first;
}

template<typename T>
T& MyVector<T>::operator[](const std::string& name) {
  DeepCopy();

  auto it = std::find_if(data_->begin(),
                         data_->end(),
                         [name](const std::pair<T, std::string>& value) {
                           return value.second == name;
                         });

  if (it == data_->end())
    throw std::invalid_argument(name + " is not found in the MyVector");

  size_t index = it - data_->begin();

  return data_->at(index).first;
}

template<typename T>
std::pair<T,
          std::string> MyVector<T>::operator[](size_t index) const {
  return data_->at(index);
}

template<typename T>
std::pair<T&, std::string&> MyVector<T>::operator[](size_t index) {
  DeepCopy();
  return data_->at(index);
}

template<typename T>
std::pair<T, std::string> MyVector<T>::at(size_t index) const {
  return data_->at(index);
}

template<typename T>
std::pair<T&, std::string&> MyVector<T>::at(size_t index) {
  return data_->at(index);
}

template<typename T>
typename std::vector<std::pair<T,
                               std::string>>::iterator MyVector<T>::begin() noexcept {
  DeepCopy();
  return data_->begin();
}

template<typename T>
typename std::vector<std::pair<T,
                               std::string>>::iterator MyVector<T>::end() noexcept {
  DeepCopy();
  return data_->end();
}

template<typename T>
typename std::vector<std::pair<T,
                               std::string>>::const_iterator MyVector<T>::cbegin() const noexcept {
  return data_->cbegin();
}

template<typename T>
typename std::vector<std::pair<T,
                               std::string>>::const_iterator MyVector<T>::cend() const noexcept {
  return data_->cend();
}

template<typename T>
bool MyVector<T>::empty() const noexcept {
  return data_->empty();
}

template<typename T>
size_t MyVector<T>::size() const noexcept {
  return data_->size();
}

template<typename T>
size_t MyVector<T>::capacity() const noexcept {
  return data_->capacity();
}

template<typename T>
void MyVector<T>::reserve(size_t size) {
  DeepCopy();
  data_->reserve(size);
}

template<typename T>
void MyVector<T>::clear() noexcept {
  DeepCopy();
  data_->clear();
}

template<typename T>
void MyVector<T>::DeepCopy() {
  if (data_.unique()) return;

  data_ = std::make_shared<vector_ptr>(*data_);
}

#endif //TASK_2_CODEREVIEW__MYVECTOR_H_
