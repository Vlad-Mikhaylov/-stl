#ifndef ITEROPS_H
#define ITEROPS_H
#include <iostream>
template <class IteratorType>
void Advance(IteratorType& iter, int n) {
  using Category = typename std::iterator_traits<IteratorType>::iterator_category;
  if constexpr (std::is_base_of<std::random_access_iterator_tag, Category>) {
    iter += n;
  } else if constexpr (std::is_base_of<std::bidirectional_iterator_tag, Category>) {
    if (n > 0) {
      while (n-- > 0) {
        ++iter;
      }
    } else {
      while (n++ < 0) {
        --iter;
      }
    }
  } else {
    while (n-- > 0) {
      ++iter;
    }
  }
}

template <class IteratorType>
IteratorType Next(IteratorType iter, int n) {
  using Category = typename std::iterator_traits<IteratorType>::iterator_category;
  if constexpr (std::is_base_of<std::random_access_iterator_tag, Category>) {
    iter += n;
    return iter;
  } else if constexpr (std::is_base_of<std::bidirectional_iterator_tag, Category>) {
    if (n > 0) {
      while (n-- > 0) {
        ++iter;
      }
      return iter;
    }
    while (n++ < 0) {
      --iter;
    }
    return iter;
  }
  while (n-- > 0) {
    ++iter;
  }
  return iter;
}

template <class IteratorType>
IteratorType Next(IteratorType iter) {
  return Next(iter, 1);
}

template <class IteratorType>
IteratorType Prev(IteratorType iter, int n) {
  using Category = typename std::iterator_traits<IteratorType>::iterator_category;
  if constexpr (std::is_base_of<std::random_access_iterator_tag, Category>) {
    iter -= n;
    return iter;
  } else if constexpr (std::is_base_of<Category, std::bidirectional_iterator_tag>) {
    if (n > 0) {
      while (n-- > 0) {
        --iter;
      }
      return iter;
    }
    while (n++ < 0) {
      ++iter;
    }
    return iter;
  }
  while (n-- > 0) {
    --iter;
  }
  return iter;
}

template <class IteratorType>
IteratorType Prev(IteratorType iter) {
  return Prev(iter, 1);
}

template <class IteratorType>
int Distance(const IteratorType& beg, const IteratorType& end) {
  IteratorType tmp = beg;
  using Category = typename std::iterator_traits<IteratorType>::iterator_category;
  if constexpr (std::is_base_of<std::random_access_iterator_tag, Category>) {
    return end - beg;
  } else {
    int n = 0;
    while (tmp != end) {
      ++n;
      ++tmp;
    }
    return n;
  }
}
#endif
