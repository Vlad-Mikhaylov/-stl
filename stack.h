#ifndef STACK_H
#define STACK_H
#include <deque>

template <class T, class Container = std::deque<T>>
class Stack {
 public:
  Container body_;
  Stack() = default;
  explicit Stack(const Container& other) : body_(Container(other.begin(), other.end())) {
  }
  template <class Iterator>
  Stack(Iterator begin, Iterator end) : body_(Container(begin, end)) {
  }
  T& Top() {
    return body_.back();
  }
  const T& Top() const {
    return body_.back();
  }
  bool Empty() {
    return body_.empty();
  }
  bool Empty() const {
    return body_.empty();
  }
  size_t Size() const {
    return body_.size();
  }
  size_t Size() {
    return body_.size();
  }
  void Push(const T& value) {
    body_.push_back(value);
  }
  template <class... Args>
  void Emplace(Args&&... args) {
    body_.emplace_back(std::forward<Args>(args)...);
  }
  void Push(T&& value) {
    body_.push_back(std::move(value));
  }
  void Pop() {
    body_.pop_back();
  }
  void Swap(Stack& other) {
    std::swap(this->body_, other.body_);
  }
};
#endif