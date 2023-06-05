#ifndef FROZEN_SET_H
#define FROZEN_SET_H
#include <optional>
#include <vector>
#include <random>

template <typename KeyT, typename HashT, typename EqualT = std::equal_to<KeyT>>
class FrozenSet {
 public:
  using KeyType = KeyT;
  using SizeType = size_t;
  using Hasher = HashT;
  using KeyEqual = EqualT;
  using Reference = KeyT&;
  using ConstReference = const KeyT&;
  using Pointer = KeyT*;
  using ConstPointer = const KeyT*;

 private:
  SizeType size_ = 0;
  Hasher hash_;
  std::vector<std::optional<KeyType>> elements_;
  auto Hash(const KeyType& key) const noexcept {
    return hash_(key);
  }
  static bool Equal(const KeyType& a, const KeyType& b) noexcept {
    static KeyEqual equal;
    return equal(a, b);
  }
  SizeType BucketId(const KeyType& key) const noexcept {
    return Hash(key) % elements_.size();
  }
  template <class InputIterator>
  bool HasCollisions(const InputIterator& begin, const InputIterator& end) const {
    size_t cur_size = elements_.size();
    std::vector<bool> filled(cur_size, false);
    for (auto it = begin; it != end; ++it) {
      size_t bucket_id = BucketId(*it);
      if (filled[bucket_id]) {
        return true;
      }
      filled[bucket_id] = true;
    }
    return false;
  }
  template <class ForwardIterator>
  void FillBucket(const ForwardIterator& begin, const ForwardIterator& end, std::mt19937_64& generator) {
    size_t cur_size = std::distance(begin, end);
    size_t bucket_size = cur_size * cur_size;
    elements_.resize(bucket_size);
    while (true) {
      hash_ = Hasher::GenerateHash(generator);
      if (!HasCollisions(begin, end)) {
        break;
      }
    }
    FillElements(begin, end);
  }
  template <class InputIterator>
  void FillElements(const InputIterator& begin, const InputIterator& end) {
    for (auto it = begin; it != end; ++it) {
      size_t bucket_id = BucketId(*it);
      ++size_;
      elements_[bucket_id] = *it;
    }
  }

 public:
  FrozenSet() = default;
  FrozenSet(const FrozenSet& other) = default;
  template <typename ForwardIterator>
  FrozenSet(const ForwardIterator& begin, const ForwardIterator& end, std::mt19937_64& generator) {
    FillBucket(begin, end, generator);
  }
  SizeType Size() const {
    return size_;
  }
  bool Find(const KeyType& key) const noexcept {
    if (elements_.empty()) {
      return false;
    }
    size_t bucket_id = BucketId(key);
    const auto element = elements_[bucket_id];
    return element.has_value() && Equal(*element, key);
  }
  bool Empty() const {
    return Size() == 0;
  }
  void Clear() {
    elements_.clear();
    size_ = 0;
  }
  template <class ForwardIterator>
  void Assign(const ForwardIterator& begin, const ForwardIterator& end, std::mt19937_64& generator) {
    Clear();
    FillBucket(begin, end, generator);
  }
};
#endif
