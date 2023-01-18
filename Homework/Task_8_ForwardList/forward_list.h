#ifndef TASK_8_FORWARDLIST__FORWARD_LIST_H_
#define TASK_8_FORWARDLIST__FORWARD_LIST_H_

#include <utility>
#include <iterator>

template<typename T>
class ForwardList {
 private:
  struct Node {
    T* value_;
    Node* next_;

    explicit Node(Node* next = nullptr)
        : value_(nullptr), next_(next) {}
    explicit Node(const T& value, Node* next = nullptr)
        : value_(new T(value)), next_(next) {}
    explicit Node(T&& value, Node* next = nullptr)
        : value_(new T(std::forward<T>(value))), next_(next) {}
    template<typename... Args>
    explicit Node(Node* next, Args&& ... args)
        : value_(new T(std::forward<Args>(args)...)), next_(next) {}
    ~Node() {
      delete value_;
    }
  };

  template<bool IsConst>
  class CommonIterator {
   public:
    using value_type = T;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;

    explicit CommonIterator(Node* node = nullptr) : node(node) {}

    CommonIterator(const CommonIterator& other) : node(other.node) {}

    CommonIterator& operator=(const CommonIterator& other) {
      if (this == &other) {
        return *this;
      }
      node = other.node;
      return *this;
    }

    template<bool Const_ = IsConst>
    std::enable_if_t<!Const_, T&> operator*() {
      return *(node->value_);
    }

    template<bool Const_ = IsConst>
    std::enable_if_t<Const_, const T&> operator*() {
      return *(node->value_);
    }

    template<bool Const_ = IsConst>
    std::enable_if_t<!Const_, T*> operator->() {
      return node->value_;
    }

    template<bool Const_ = IsConst>
    std::enable_if_t<Const_, const T*> operator->() {
      return node->value_;
    }

    CommonIterator<IsConst>& operator++() {
      if (!node || node->next_ == nullptr) {
        throw std::out_of_range("Iterator can't be incremented");
      }

      node = node->next_;
      return *this;
    }
    CommonIterator<IsConst> operator++(int) {
      auto old_iterator = CommonIterator<IsConst>(node);
      node = node->next_;
      return old_iterator;
    }

    bool operator==(const CommonIterator<IsConst>& other) {
      return (node == other.node);
    }

    bool operator!=(const CommonIterator<IsConst>& other) {
      return (node != other.node);
    }

    Node* node;
  };

  Node* head_ = nullptr;
  Node* tail_ = nullptr;
  size_t length_ = 0;
 public:
  using value_type = T;

  ForwardList();
  explicit ForwardList(size_t count, const T& value = T());
  ForwardList(std::initializer_list<T> ilist);
  template<class InputIt>
  ForwardList(InputIt first, InputIt last);
  ForwardList(const ForwardList<T>& other);
  ForwardList(ForwardList<T>&& other) noexcept;

  ForwardList<T>& operator=(const ForwardList<T>& other);
  ForwardList<T>& operator=(ForwardList<T>&& other) noexcept;

  ~ForwardList();

  size_t size() const;

  T& front();

  void clear();
  bool empty();

  using iterator = CommonIterator<false>;
  using const_iterator = CommonIterator<true>;

  iterator begin() const;
  iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

  iterator insert_after(iterator pos, const T& value);
  iterator insert_after(iterator pos, T&& value);
  iterator insert_after(iterator pos, size_t count, const T& value);
  template<typename InputIt>
  iterator insert_after(iterator pos, InputIt first, InputIt last);
  iterator insert_after(iterator pos, std::initializer_list<T> ilist);

  template<class... Args>
  iterator emplace_after(iterator pos, Args&& ... args);

  iterator erase_after(iterator pos);
  iterator erase_after(iterator first, iterator last);

  void push_front(const T& value);
  void push_front(T&& value);

  template<class... Args>
  void emplace_front(Args&& ... args);

  void pop_front();

  void swap(ForwardList<T>& other);

};

template<typename T>
ForwardList<T>::ForwardList() : tail_(new Node), length_(0) {
  head_ = tail_;
}

template<typename T>
ForwardList<T>::ForwardList(size_t count, const T& value) : ForwardList() {
  for (size_t i = 0; i < count; ++i) {
    insert_after(begin(), value);
  }
}

template<typename T>
ForwardList<T>::ForwardList(std::initializer_list<T> ilist): ForwardList() {
  auto it = begin();
  for (const auto& item : ilist) {
    it = insert_after(it, item);
  }
}

template<typename T>
template<class InputIt>
ForwardList<T>::ForwardList(InputIt first, InputIt last) : ForwardList() {
  auto it = begin();
  for (auto curr = first; curr != last; ++curr) {
    it = insert_after(it, *curr);
  }
}

template<typename T>
ForwardList<T>::ForwardList(const ForwardList<T>& other) : ForwardList() {
  insert_after(begin(), other.begin(), other.end());
}

template<typename T>
ForwardList<T>::ForwardList(ForwardList<T>&& other) noexcept :
    head_(other.head_), tail_(other.tail_), length_(other.length_) {
  other.head_ = new Node();
  other.tail_ = other.head_;
  other.length_ = 0;
}

template<typename T>
ForwardList<T>& ForwardList<T>::operator=(const ForwardList<T>& other) {
  if (this == &other) {
    return *this;
  }

  auto copy = other;
  swap(copy);

  return *this;
}

template<typename T>
ForwardList<T>& ForwardList<T>::operator=(ForwardList<T>&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  clear();
  delete tail_;

  head_ = other.head_;
  tail_ = other.tail_;
  length_ = other.length_;

  other.head_ = new Node;
  other.tail_ = other.head_;
  other.length_ = 0;

  return *this;
}

template<typename T>
ForwardList<T>::~ForwardList() {
  Node* current = head_;
  while (current != nullptr) {
    Node* next = current->next_;
    delete current;
    current = next;
  }
}

template<typename T>
size_t ForwardList<T>::size() const {
  return length_;
}

template<typename T>
T& ForwardList<T>::front() {
  return *(begin());
}

template<typename T>
void ForwardList<T>::clear() {
  auto it = head_;
  while (it != tail_) {
    auto next = it->next_;
    delete it;
    it = next;
  }
  head_ = tail_;
  length_ = 0;
}

template<typename T>
bool ForwardList<T>::empty() {
  return length_ == 0;
}

template<typename T>
typename ForwardList<T>::iterator ForwardList<T>::begin() const {
  return iterator(head_);
}

template<typename T>
typename ForwardList<T>::iterator ForwardList<T>::end() const {
  return iterator(tail_);
}

template<typename T>
typename ForwardList<T>::const_iterator ForwardList<T>::cbegin() const {
  return const_iterator(head_);
}

template<typename T>
typename ForwardList<T>::const_iterator ForwardList<T>::cend() const {
  return const_iterator(head_);
}

template<typename T>
typename ForwardList<T>::iterator ForwardList<T>::insert_after(
    ForwardList::iterator pos,
    const T& value) {
  auto new_node = new Node(value, pos.node->next_);

  if (head_ == tail_) {
    new_node->next_ = tail_;
    head_ = new_node;
  } else {
    pos.node->next_ = new_node;
  }
  ++length_;

  return iterator(new_node);
}

template<typename T>
typename ForwardList<T>::iterator ForwardList<T>::insert_after(
    ForwardList::iterator pos,
    T&& value) {
  auto new_node = new Node(std::move(value), pos.node->next_);

  if (head_ == tail_) {
    new_node->next_ = tail_;
    head_ = new_node;
  } else {
    pos.node->next_ = new_node;
  }
  ++length_;

  return iterator(new_node);
}

template<typename T>
typename ForwardList<T>::iterator ForwardList<T>::insert_after(
    ForwardList::iterator pos,
    size_t count,
    const T& value) {

  for (size_t i = 0; i < count; ++i) {
    pos = insert_after(pos, value);
  }

  return pos;
}

template<typename T>
template<typename InputIt>
typename ForwardList<T>::iterator ForwardList<T>::insert_after(
    ForwardList::iterator pos,
    InputIt first,
    InputIt last) {

  for (auto it = first; it != last; ++it) {
    pos = insert_after(pos, *it);
  }

  return pos;
}

template<typename T>
typename ForwardList<T>::iterator ForwardList<T>::insert_after(
    ForwardList::iterator pos,
    std::initializer_list<
        T> ilist) {
  auto it = insert_after(pos, ilist.begin(), ilist.end());

  return it;
}

template<typename T>
template<typename ... Args>
typename ForwardList<T>::iterator ForwardList<T>::emplace_after(
    iterator pos,
    Args&& ...args) {
  auto new_node = new Node(pos.node->next_, std::forward<Args>(args)...);

  if (head_ == tail_) {
    new_node->next_ = tail_;
    head_ = new_node;
  } else {
    pos.node->next_ = new_node;
  }
  ++length_;

  return iterator(new_node);
}

template<typename T>
typename ForwardList<T>::iterator ForwardList<T>::erase_after(
    ForwardList::iterator pos) {
  if (head_ == tail_ || pos.node->next_ == tail_) return end();

  auto node_to_remove = pos.node->next_;
  pos.node->next_ = node_to_remove->next_;
  delete node_to_remove;
  --length_;

  return iterator(pos.node->next_);
}

template<typename T>
typename ForwardList<T>::iterator ForwardList<T>::erase_after(ForwardList::iterator first,
                                                              ForwardList::iterator last) {
  auto it = first;
  while (it != end() && it != last) {
    it = erase_after(it);
  }

  return it;
}

template<typename T>
void ForwardList<T>::push_front(const T& value) {
  auto new_head = new Node(value, head_);

  head_ = new_head;
  ++length_;
}

template<typename T>
void ForwardList<T>::push_front(T&& value) {
  auto new_head = new Node(std::move(value), head_);

  head_ = new_head;
  ++length_;
}

template<typename T>
template<class... Args>
void ForwardList<T>::emplace_front(Args&& ... args) {
  auto new_head = new Node(std::forward<Args>(args)..., head_);

  head_ = new_head;
  ++length_;
}

template<typename T>
void ForwardList<T>::pop_front() {
  if (head_ == tail_) return;

  auto new_head = head_->next_;
  delete head_;
  head_ = new_head;
}

template<typename T>
void ForwardList<T>::swap(ForwardList<T>& other) {
  std::swap(head_, other.head_);
  std::swap(tail_, other.tail_);
  std::swap(length_, other.length_);
}

#endif //TASK_8_FORWARDLIST__FORWARD_LIST_H_
