#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

template <class T>
class UniquePtr {
 public:
  T* ptr_;
  UniquePtr() {
    this->ptr_ = nullptr;
  }
  explicit UniquePtr(T* other_ptr) : ptr_(other_ptr) {
  }
  UniquePtr(const T& other) = delete;
  UniquePtr& operator=(UniquePtr& other) = delete;
  UniquePtr(UniquePtr&& other) noexcept {
    this->ptr_ = other.ptr_;
    other.ptr_ = nullptr;
  }
  ~UniquePtr() {
    delete this->ptr_;
  }
  UniquePtr& operator=(UniquePtr&& other) noexcept {
    if (this->ptr_ != other.ptr_) {
      delete ptr_;
      this->ptr_ = other.ptr_;
      other.ptr_ = nullptr;
    }
    return *this;
  }
  T* Release() {
    T* tmp = this->ptr_;
    this->ptr_ = nullptr;
    return tmp;
  }
  void Reset(T* other = nullptr) {
    delete this->ptr_;
    this->ptr_ = other;
  }
  void Swap(UniquePtr<T>& other) {
    T* tmp = this->ptr_;
    this->ptr_ = other.ptr_;
    other.ptr_ = tmp;
  }
  T* Get() {
    return this->ptr_;
  }
  T* Get() const {
    return this->ptr_;
  }
  T& operator*() const {
    return *ptr_;
  }
  T& operator*() {
    return *ptr_;
  }
  T* operator->() const {
    return ptr_;
  }
  T* operator->() {
    return ptr_;
  }
  explicit operator bool() const {
    return this->ptr_ != nullptr ? true : false;
  }
};
#endif