#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <type_traits>
#include <utility>
#include <stdexcept>

class BadOptionalAccess : public std::runtime_error {  // NOLINT
 public:
  BadOptionalAccess() : std::runtime_error("BadOptionalAccess") {
  }
};

template <class T>
class Optional {
 public:
  std::aligned_storage_t<sizeof(T), alignof(T)> storage_;
  bool has_value_;
  T* value_;

  Optional() : has_value_(false) {
  }
  Optional(const Optional& other) {
    if (other.has_value_) {
      this->value_ = new (&storage_) T(*other.value_);
      this->has_value_ = other.has_value_;
    } else {
      has_value_ = false;
    }
  }
  Optional(Optional&& other) noexcept {
    if (other.has_value_) {
      this->storage_ = other.storage_;
      this->value_ = new (&storage_) T(std::move(*(other.value_)));
      this->has_value_ = other.has_value_;
    } else {
      has_value_ = false;
    }
  }
  explicit Optional(const T& object) {
    this->value_ = new (&storage_) T(std::move(object));
    this->has_value_ = true;
  }
  explicit Optional(T&& object) {
    this->value_ = new (&storage_) T(std::move(object));
    this->has_value_ = true;
  }
  ~Optional() {
    if (this->has_value_) {
      (this->value_)->~T();
    }
    this->has_value_ = false;
  }
  Optional& operator=(const Optional& other) {  // kostyl
    if (this != &other) {
      if (this->has_value_) {
        (this->value_)->~T();
      }
      this->has_value_ = other.has_value_;
      if (other.has_value_) {
        this->value_ = new (&storage_) T(*other.value_);
      }
    }
    return *this;
  }
  Optional& operator=(Optional&& other) noexcept {
    if (this != &other) {
      if (this->has_value_) {
        (this->value_)->~T();
      }
      this->has_value_ = other.has_value_;
      if (other.has_value_) {
        this->value_ = new (&storage_) T(std::move(*(other.value_)));
      }
    }
    return *this;
  }
  Optional& operator=(const T& object) noexcept {
    if (this->has_value_) {
      (this->value_)->~T();
    }
    this->value_ = new (&storage_) T(std::move(object));
    this->has_value_ = true;
    return *this;
  }
  Optional& operator=(T&& object) noexcept {
    if (this->has_value_) {
      (this->value_)->~T();
    }
    this->has_value_ = true;
    this->value_ = new (&storage_) T(std::move(object));
    return *this;
  }
  bool HasValue() const {
    return this->has_value_;
  }
  explicit operator bool() const {
    return this->has_value_;
  }
  T& Value() {
    if (!has_value_) {
      throw BadOptionalAccess{};
    }
    return *value_;
  }
  const T& Value() const {
    if (!has_value_) {
      throw BadOptionalAccess{};
    }
    return *value_;
  }
  const T& operator*() const {
    return *value_;
  }
  T& operator*() {
    return *value_;
  }
  template <class... Args>
  T& Emplace(Args&&... args) {
    if (this->has_value_) {
      (this->value_)->~T();
    }
    this->value_ = new (&storage_) T(std::forward<Args>(args)...);
    this->has_value_ = true;
    return *value_;
  }
  void Reset() {
    if (has_value_) {
      this->~Optional();
    }
    this->has_value_ = false;
  }
  template <class V>
  void Swap(Optional<V>& rhs) {
    Optional<T> tmp = std::move(*this);
    this->Reset();
    *this = std::move(rhs);
    rhs = std::move(tmp);
    tmp.Reset();
  }
};

#endif