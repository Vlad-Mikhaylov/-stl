#ifndef ISPRIME
#define ISPRIME
#include <iostream>

template <uint64_t F, uint64_t L, uint64_t R>
struct Sqrt;

template <uint64_t Arg, uint64_t Left = 1, uint64_t Right = Arg>
inline const uint64_t kSqrtV = Sqrt<Arg, Left, Right>::value;

template <uint64_t N, uint64_t L, uint64_t R>
struct Sqrt
    : std::integral_constant<
          uint64_t, (R - L > 1 ? kSqrtV<N, ((((L + R + 1) / 2) < N / ((L + R + 1) / 2)) ? ((L + R + 1) / 2) : L),
                                        ((((L + R + 1) / 2) < N / ((L + R + 1) / 2)) ? R : ((L + R + 1) / 2))>
                               : (R * R > N ? L : R))> {};

template <uint64_t Arg, uint64_t F>
struct Sqrt<Arg, F, F> : std::integral_constant<uint64_t, F> {};

template <>
struct Sqrt<0, 1, 0> : std::integral_constant<uint64_t, 0> {};

template <uint64_t Arg, uint64_t L, uint64_t R, uint64_t Mid = (R + L + 1) / 2>
struct HasDivisor
    : std::integral_constant<bool, std::disjunction_v<HasDivisor<Arg, Mid, R>, HasDivisor<Arg, L, Mid - 1>>> {};

template <uint64_t Arg, uint64_t F>
struct HasDivisor<Arg, F, F, F> : std::integral_constant<bool, ((Arg % F) == 0)> {};

template <uint64_t Arg, uint64_t L, uint64_t R>
inline const bool kHasDivisorOnV = HasDivisor<Arg, L, R>::value;

template <uint64_t Arg>
inline const bool kIsPrimeV = !kHasDivisorOnV<Arg, 2, kSqrtV<Arg>>;

template <>
inline const bool kIsPrimeV<1> = false;

template <>
inline const bool kIsPrimeV<2> = true;

template <>
inline const bool kIsPrimeV<3> = true;

template <>
inline const bool kIsPrimeV<4> = false;

template <>
inline const bool kIsPrimeV<5> = true;

template <>
inline const bool kIsPrimeV<6> = false;
#endif
