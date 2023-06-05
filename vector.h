)))#pragma once
#include <iterator>

#define VECTOR_MEMORY_IMPLEMENTED

template <class Type>
class Vector {
 private:
  size_t capacity_;
  size_t size_;
  Type* buffer_;

 public:
  using ValueType = Type;
  using Pointer = Type*;
  using ConstPointer = const Type*;
  using Reference = Type&;
  using ConstReference = const Type&;
  using SizeType = const size_t&;
  using Iterator = Type*;
  using ConstIterator = const Type*;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

  Vector() {
    capacity_ = 0;
    size_ = 0;
    buffer_ = nullptr;
  }

  explicit Vector(SizeType size) {
    if (size == 0) {  // no memory allocation
      capacity_ = 0;
      size_ = 0;
      buffer_ = nullptr;

    } else {
      auto new_buffer = static_cast<Pointer>(operator new(sizeof(Type) * size));
      // static_cast is necessary

      size_t index = 0;
      // it's necessary to save index for try-catch block to delete constructed objects
      // (new_buffer[index] - the last constructed object in new_buffer)

      try {
        // construct objects (size)
        while (index < size) {
          new (new_buffer + index) Type{};
          ++index;
        }
      } catch (...) {
        // delete constructed objects + deallocate memory
        for (size_t i = 0; i <= index; ++i) {
          (new_buffer + i)->~Type();
        }
        operator delete(new_buffer);
        throw;
      }

      capacity_ = size;
      size_ = size;
      buffer_ = new_buffer;
    }
  }

  Vector(SizeType size, ConstReference value) {
    if (size == 0) {  // no memory allocation
      capacity_ = 0;
      size_ = 0;
      buffer_ = nullptr;
    } else {
      auto new_buffer = static_cast<Pointer>(operator new(sizeof(Type) * size));
      // static_cast is necessary

      size_t index = 0;
      // it's necessary to save index for try-catch block to delete constructed objects
      // (new_buffer[index] - the last constructed object in new_buffer)

      try {
        // construct objects (size)
        while (index < size) {
          new (new_buffer + index) Type{value};
          ++index;
        }
      } catch (...) {
        // delete constructed objects + deallocate memory
        for (size_t i = 0; i <= index; ++i) {
          (new_buffer + i)->~Type();
        }
        operator delete(new_buffer);
        throw;
      }

      capacity_ = size;
      size_ = size;
      buffer_ = new_buffer;
    }
  }

  Vector(const std::initializer_list<Type>& list) {
    // comments are the same as the previous constructor
    if (!empty(list)) {
      auto size = list.size();
      auto new_buffer = static_cast<Pointer>(operator new(sizeof(Type) * size));
      size_t index = 0;

      try {
        while (index < size) {
          new (new_buffer + index) Type{*(std::next(list.begin(), index))};
          ++index;
        }
      } catch (...) {
        for (size_t i = 0; i <= index; ++i) {
          (new_buffer + i)->~Type();
        }
        operator delete(new_buffer);
        throw;
      }

      capacity_ = size;
      size_ = size;
      buffer_ = new_buffer;
    }
  }

  template <typename Iterator,
            typename = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag,
                                                          typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator second) {
    auto dist = std::distance(first, second);
    size_t size = dist > 0 ? dist : 0;

    if (size > 0) {
      auto new_buffer = static_cast<Pointer>(operator new(sizeof(Type) * size));
      size_t index = 0;

      try {
        while (index < size) {
          new (new_buffer + index) Type{*(std::next(first, index))};
          ++index;
        }
      } catch (...) {
        for (size_t i = 0; i <= index; ++i) {
          (new_buffer + i)->~Type();
        }
        operator delete(new_buffer);
        throw;
      }

      capacity_ = size;
      size_ = size;
      buffer_ = new_buffer;

    } else {
      capacity_ = 0;
      size_ = 0;
      buffer_ = nullptr;
    }
  }

  Vector(const Vector& other) {
    if (other.Empty()) {
      capacity_ = 0;
      size_ = 0;
      buffer_ = nullptr;

    } else {
      auto new_buffer = static_cast<Pointer>(operator new(sizeof(Type) * other.capacity_));
      size_t index = 0;

      try {
        while (index < other.size_) {
          new (new_buffer + index) Type{other[index]};
          ++index;
        }
      } catch (...) {
        for (size_t i = 0; i <= index; ++i) {
          (new_buffer + i)->~Type();
        }
        operator delete(new_buffer);
        throw;
      }

      capacity_ = other.capacity_;
      size_ = other.size_;
      buffer_ = new_buffer;
    }
  }

  Vector(Vector&& other) noexcept {
    capacity_ = other.capacity_;
    size_ = other.size_;
    buffer_ = other.buffer_;

    other.capacity_ = 0;
    other.size_ = 0;
    other.buffer_ = nullptr;
  }

  Vector& operator=(const Vector& other) {
    if (this != &other) {
      Vector<Type> result = other;
      this->Swap(result);
    }
    return *this;
  }

  Vector& operator=(Vector&& other) noexcept {
    Vector<Type> result = std::move(other);
    this->Swap(result);
    return *this;
  }

  ~Vector() {
    for (size_t i = 0; i < size_; ++i) {
      (buffer_ + i)->~Type();
    }
    operator delete(buffer_);
  }

  ConstReference operator[](SizeType index) const {
    return buffer_[index];
  }

  Reference operator[](SizeType index) {
    return buffer_[index];
  }

  ConstReference At(SizeType index) const {
    if (index >= size_) {
      throw std::out_of_range("Out of range");
    }
    return buffer_[index];
  }

  Reference At(SizeType index) {
    if (index >= size_) {
      throw std::out_of_range("Out of range");
    }
    return buffer_[index];
  }

  [[nodiscard]] SizeType Size() const {
    return size_;
  }

  [[nodiscard]] SizeType Capacity() const {
    return capacity_;
  }

  [[nodiscard]] bool Empty() const {
    return (size_ == 0);
  }

  ConstReference Front() const {
    return buffer_[0];
  }

  Reference Front() {
    return buffer_[0];
  }

  ConstReference Back() const {
    return buffer_[size_ - 1];
  }

  Reference Back() {
    return buffer_[size_ - 1];
  }

  ConstPointer Data() const {
    return buffer_;
  }

  Pointer Data() {
    return buffer_;
  }

  void Swap(Vector& other) {
    auto swapper = capacity_;
    capacity_ = other.capacity_;
    other.capacity_ = swapper;

    swapper = size_;
    size_ = other.size_;
    other.size_ = swapper;

    auto pointer_swapper = buffer_;
    buffer_ = other.buffer_;
    other.buffer_ = pointer_swapper;
  }

  void Resize(SizeType new_size) {
    if (new_size > capacity_) {  // expand capacity + construct default objects
      auto new_buffer = static_cast<Pointer>(operator new(sizeof(Type) * new_size));
      size_t index = 0;

      try {
        while (index < size_) {
          new (new_buffer + index) Type{std::move(buffer_[index])};
          ++index;
        }
        while (index < new_size) {
          new (new_buffer + index) Type{};
          ++index;
        }
      } catch (...) {
        for (size_t i = 0; i <= index; ++i) {
          if (i < size_) {
            buffer_[i] = std::move(new_buffer[i]);
          }
          (new_buffer + i)->~Type();
        }
        operator delete(new_buffer);
        throw;
      }

      if (buffer_ != nullptr) {
        for (size_t i = 0; i < size_; ++i) {
          (buffer_ + i)->~Type();
        }
        operator delete(buffer_);
      }

      capacity_ = new_size;
      size_ = new_size;
      buffer_ = new_buffer;

    } else if (new_size > size_) {  // only construct objects
      size_t index = size_;

      try {
        while (index < new_size) {
          new (buffer_ + index) Type{};
          ++index;
        }
      } catch (...) {
        for (size_t i = size_; i < index; ++i) {
          (buffer_ + i)->~Type();
        }
        throw;
      }
      size_ = new_size;

    } else {  // destruct objects
      for (size_t i = new_size; i < size_; ++i) {
        (buffer_ + i)->~Type();
      }
      size_ = new_size;
    }
  }

  void Resize(SizeType new_size, Type&& value) {
    if (new_size > capacity_) {  // expand capacity + construct objects
      auto new_buffer = static_cast<Pointer>(operator new(sizeof(Type) * new_size));
      size_t index = 0;

      try {
        while (index < size_) {
          new (new_buffer + index) Type{std::move(buffer_[index])};
          ++index;
        }
        while (index < new_size) {
          new (new_buffer + index) Type{std::move(value)};
          ++index;
        }
      } catch (...) {
        for (size_t i = 0; i <= index; ++i) {
          if (i < size_) {
            buffer_[i] = std::move(new_buffer[i]);
          }
          (new_buffer + i)->~Type();
        }
        operator delete(new_buffer);
        throw;
      }

      if (buffer_ != nullptr) {
        for (size_t i = 0; i < size_; ++i) {
          (buffer_ + i)->~Type();
        }
        operator delete(buffer_);
      }

      capacity_ = new_size;
      size_ = new_size;
      buffer_ = new_buffer;

    } else if (new_size > size_) {  // only construct objects
      size_t index = size_;

      try {
        while (index < new_size) {
          new (buffer_ + index) Type{std::move(value)};
          ++index;
        }
      } catch (...) {
        for (size_t i = size_; i < index; ++i) {
          (buffer_ + i)->~Type();
        }
        throw;
      }
      size_ = new_size;

    } else {  // delete objects
      for (size_t i = new_size; i < size_; ++i) {
        (buffer_ + i)->~Type();
      }
      size_ = new_size;
    }
  }

  void Reserve(SizeType new_capacity) {
    if (new_capacity > capacity_) {  // expand capacity: move elements to new buffer
      auto new_buffer = static_cast<Pointer>(operator new(sizeof(Type) * new_capacity));
      size_t index = 0;

      try {
        while (index < size_) {
          new (new_buffer + index) Type{std::move(buffer_[index])};
          ++index;
        }
      } catch (...) {
        for (size_t i = 0; i <= index; ++i) {
          buffer_[i] = std::move(new_buffer[i]);
          (new_buffer + i)->~Type();
        }
        operator delete(new_buffer);
        throw;
      }

      if (buffer_ != nullptr) {
        for (size_t i = 0; i < size_; ++i) {
          (buffer_ + i)->~Type();
        }
        operator delete(buffer_);
      }

      capacity_ = new_capacity;
      buffer_ = new_buffer;
    }
  }

  void ShrinkToFit() {
    if (Empty()) {  // destruct objects + memory deallocation
      for (size_t i = 0; i < size_; ++i) {
        (buffer_ + i)->~Type();
      }
      operator delete(buffer_);

      capacity_ = 0;
      buffer_ = nullptr;

    } else {  // move objects to new_buffer (new_capacity = size_)
      auto new_buffer = static_cast<Pointer>(operator new(sizeof(Type) * size_));
      size_t index = 0;

      try {
        while (index < size_) {
          new (new_buffer + index) Type;
          new_buffer[index] = std::move(buffer_[index]);
          ++index;
        }
      } catch (...) {
        for (size_t i = 0; i <= index; ++i) {
          (new_buffer + i)->~Type();
        }
        operator delete(new_buffer);
        throw;
      }

      if (buffer_ != nullptr) {
        for (size_t i = 0; i < size_; ++i) {
          (buffer_ + i)->~Type();
        }
        operator delete(buffer_);
      }

      capacity_ = size_;
      buffer_ = new_buffer;
    }
  }

  void Clear() {
    // destruct objects, but memory doesn't deallocated
    for (size_t i = 0; i < size_; ++i) {
      (buffer_ + i)->~Type();
    }
    size_ = 0;
  }

  void PushBack(Type&& value) {
    if (size_ == capacity_) {  // need to expand capacity (move_objects to new buffer)
      size_t new_capacity = (capacity_ == 0) ? 1 : 2 * capacity_;
      auto new_buffer = static_cast<Pointer>(operator new(sizeof(Type) * new_capacity));
      size_t index = 0;

      try {
        while (index < size_) {
          new (new_buffer + index) Type{std::move(buffer_[index])};
          ++index;
        }
      } catch (...) {
        for (size_t i = 0; i <= index; ++i) {
          buffer_[i] = std::move(new_buffer[i]);
          (new_buffer + i)->~Type();
        }
        operator delete(new_buffer);
        throw;
      }

      // add new element
      try {
        new (new_buffer + size_) Type{std::move(value)};
      } catch (...) {
        for (size_t i = 0; i < index; ++i) {
          buffer_[i] = std::move(new_buffer[i]);
          (new_buffer + i)->~Type();
        }
        (new_buffer + index)->~Type();
        operator delete(new_buffer);
        throw;
      }

      // old memory deallocation
      if (buffer_ != nullptr) {
        for (size_t i = 0; i < size_; ++i) {
          (buffer_ + i)->~Type();
        }
        operator delete(buffer_);
      }

      capacity_ = new_capacity;
      ++size_;
      buffer_ = new_buffer;

    } else {
      try {
        new (buffer_ + size_) Type{std::move(value)};
        ++size_;
      } catch (...) {
        (buffer_ + size_)->~Type();
        throw;
      }
    }
  }

  void PushBack(ConstReference value) {
    if (size_ == capacity_) {  // need to expand capacity (move_objects to new buffer)
      size_t new_capacity = (capacity_ == 0) ? 1 : 2 * capacity_;
      auto new_buffer = static_cast<Pointer>(operator new(sizeof(Type) * new_capacity));
      size_t index = 0;

      try {
        while (index < size_) {
          new (new_buffer + index) Type{std::move(buffer_[index])};
          ++index;
        }
      } catch (...) {
        for (size_t i = 0; i <= index; ++i) {
          buffer_[i] = std::move(new_buffer[i]);
          (new_buffer + i)->~Type();
        }
        operator delete(new_buffer);
        throw;
      }

      // add new element
      try {
        new (new_buffer + size_) Type{value};
      } catch (...) {
        for (size_t i = 0; i < index; ++i) {
          buffer_[i] = std::move(new_buffer[i]);
          (new_buffer + i)->~Type();
        }
        (new_buffer + index)->~Type();
        operator delete(new_buffer);
        throw;
      }

      // old memory deallocation
      if (buffer_ != nullptr) {
        for (size_t i = 0; i < size_; ++i) {
          (buffer_ + i)->~Type();
        }
        operator delete(buffer_);
      }

      capacity_ = new_capacity;
      ++size_;
      buffer_ = new_buffer;

    } else {  // if we can push the element in the current buffer
      try {
        new (buffer_ + size_) Type{value};
        ++size_;
      } catch (...) {
        (buffer_ + size_)->~Type();
        throw;
      }
    }
  }

  void PopBack() {
    if (size_ > 0) {
      (buffer_ + size_ - 1)->~Type();
      --size_;
    }
  }

  template <typename... Args>
  void EmplaceBack(Args&&... args) {
    this->PushBack(Type(std::forward<Args>(args)...));
  }

  friend bool operator<(const Vector& lhs, const Vector& rhs) {
    // obvious logic
    size_t min_size = (lhs.size_ < rhs.size_) ? lhs.size_ : rhs.size_;

    for (size_t i = 0; i < min_size; ++i) {
      if (lhs[i] != rhs[i]) {
        return lhs[i] < rhs[i];
      }
    }
    return lhs.size_ < rhs.size_;
  }

  friend bool operator>(const Vector& lhs, const Vector& rhs) {
    return (rhs < lhs);
  }

  friend bool operator<=(const Vector& lhs, const Vector& rhs) {
    return !(rhs < lhs);
  }

  friend bool operator>=(const Vector& lhs, const Vector& rhs) {
    return !(lhs < rhs);
  }

  friend bool operator!=(const Vector& lhs, const Vector& rhs) {
    return (lhs < rhs || rhs < lhs);
  }

  friend bool operator==(const Vector& lhs, const Vector& rhs) {
    return !(lhs != rhs);
  }

  Iterator begin() {  //  NOLINT
    return Iterator{buffer_};
  }

  Iterator end() {  //  NOLINT
    return Iterator{buffer_ + size_};
  }

  ConstIterator begin() const {  //  NOLINT
    return ConstIterator{buffer_};
  }

  ConstIterator end() const {  //  NOLINT
    return ConstIterator{buffer_ + size_};
  }

  ConstIterator cbegin() const {  //  NOLINT
    return ConstIterator{buffer_};
  }

  ConstIterator cend() const {  //  NOLINT
    return ConstIterator{buffer_ + size_};
  }

  ReverseIterator rbegin() {  //  NOLINT
    return std::make_reverse_iterator(this->end());
  }

  ReverseIterator rend() {  //  NOLINT
    return std::make_reverse_iterator(this->begin());
  }

  ConstReverseIterator rbegin() const {  //  NOLINT
    return std::make_reverse_iterator(this->cend());
  }

  ConstReverseIterator rend() const {  //  NOLINT
    return std::make_reverse_iterator(this->cbegin());
  }

  ConstReverseIterator crbegin() const {  //  NOLINT
    return std::make_reverse_iterator(this->cend());
  }

  ConstReverseIterator crend() const {  //  NOLINT
    return std::make_reverse_iterator(this->cbegin());
  }
};
