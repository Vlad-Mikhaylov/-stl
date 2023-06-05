#include <iostream>
#ifndef ARRAY_TRAITS
#define ARRAY_TRAITS

template <class T>
struct kIsArray : std::integral_constant<bool, false> {};  // NOLINT

template <class T, size_t N>
struct kIsArray<T[N]> : std::integral_constant<bool, true> {};

template <class T>
struct kIsArray<T[]> : std::integral_constant<bool, true> {};

template <class T>
inline constexpr bool kIsArrayV = kIsArray<T>::value;

template <class T>
struct kTotalSize : std::integral_constant<size_t, 1> {};  // NOLINT

template <class T, size_t N, size_t M>
struct kTotalSize<T[N][M]> : std::integral_constant<size_t, N * kTotalSize<T[M]>::value>{};

template <class T, size_t N>
struct kTotalSize<T[N]> : std::integral_constant<size_t, N> {};

template <class T>
struct kTotalSize<T[]> : std::integral_constant<size_t, 0> {};

template <class T>
inline constexpr size_t kTotalSizeV = kTotalSize<T>::value;

template <class T>
struct kRank : std::integral_constant<size_t, 0> {};  // NOLINT

template <class T, size_t N>
struct kRank<T[N]> : std::integral_constant<size_t, kRank<T>::value + 1> {};

template <class T>
struct kRank<T[]> : std::integral_constant<size_t, kRank<T>::value + 1> {};

template <class T>
inline constexpr size_t kRankV = kRank<T>::value;

template <class T>
struct kSize : std::integral_constant<size_t, 1> {};  // NOLINT

template <class T, size_t N>
struct kSize<T[N]> : std::integral_constant<size_t, N> {};

template <class T>
struct kSize<T[]> : std::integral_constant<size_t, 0> {};

template <class T>
inline constexpr size_t kSizeV = kSize<T>::value;

template <class T>
struct RemoveArray {
  using ValueType = T;
};

template <class T, size_t N>
struct RemoveArray<T[N]> {
  using ValueType = T;
};

template <class T>
struct RemoveArray<T[]> {
  using ValueType = T;
};

template <class T>
using RemoveArrayT = typename RemoveArray<T>::ValueType;

template <class T>
struct RemoveAllArrays {
  using ValueType = T;
};

template <class T, size_t N>
struct RemoveAllArrays<T[N]> {
  using ValueType = typename RemoveAllArrays<T>::ValueType;
};

template <class T>
struct RemoveAllArrays<T[]> {
  using ValueType = typename RemoveAllArrays<T>::ValueType;
};

template <class T>
using RemoveAllArraysT = typename RemoveAllArrays<T>::ValueType;
#endif
