#ifndef RANGE_H_
#define RANGE_H_
#define REVERSE_RANGE_IMPLEMENTED

class Iterator {
 private:
  int value_;
  int step_;

 public:
  Iterator(int val, int step) : value_(val), step_(step) {  //  NOLINT
  }
  void operator++() {
    value_ = value_ + step_;
  }
  int operator*() const {
    return value_;
  }
  friend bool operator!=(const Iterator& other1, const Iterator& other2) {
    return ((other2.value_ - other1.value_) * other1.step_ > 0);
  }
};

class ReverseIterator {
 public:
  int value_;
  int step_;
  explicit ReverseIterator(int value, int step) : value_(value), step_(step) {
  }
  void operator++() {
    value_ = value_ - step_;
  }
  int operator*() {
    return value_;
  }
  friend bool operator!=(const ReverseIterator& other1, const ReverseIterator& other2) {
    if (other1.step_ == 0) {
      return false;
    }
    return ((other2.value_ - other1.value_) * other1.step_ <= 0);
  }
};

class Object {
 public:
  int begin_ = 0;
  int end_ = 0;
  int step_ = 1;
  Object() = default;
  Object(int end) : begin_(0), end_(end), step_(1) {  //  NOLINT
  }
  Object(int begin, int end) : begin_(begin), end_(end), step_(1) {
  }
  Object(int begin, int end, int step) : begin_(begin), end_(end), step_(step) {
    if (step_ == 0) {
      end_ = begin_;
    }
  }
  Iterator begin() {  // NOLINT
    return Iterator(begin_, step_);
  }
  Iterator end() {  // NOLINT
    return Iterator(end_, step_);
  }
  ReverseIterator rbegin() {  // NOLINT
    if (step_ != 0) {
      if ((((end_ % step_) - (begin_ % step_)) % step_) != 0) {
        return ReverseIterator((end_ - begin_ + step_ - 1) / step_ * step_ + begin_ - step_, step_);
      }
    }
    return ReverseIterator(end_ - step_, step_);
  }
  ReverseIterator rend() {  // NOLINT
    return ReverseIterator(begin_, step_);
  }
  const Iterator begin() const {  // NOLINT
    return Iterator(begin_, step_);
  }
  const Iterator end() const {  // NOLINT
    return Iterator(end_, step_);
  }
  const ReverseIterator rbegin() const {  // NOLINT
    if (step_ != 0) {
      if ((((end_ % step_) - (begin_ % step_)) % step_) != 0) {
        return ReverseIterator((end_ - begin_ + step_ - 1) / step_ * step_ + begin_ - step_, step_);
      }
    }
    return ReverseIterator(end_ - step_, step_);
  }
  const ReverseIterator rend() const {  // NOLINT
    return ReverseIterator(begin_, step_);
  }
};

Object inline Range(int begin, int end, int step) {
  return Object(begin, end, step);
}
Object inline Range(int begin, int end) {
  return Object(begin, end);
}
Object inline Range(int end) {
  return Object(end);
}
#endif