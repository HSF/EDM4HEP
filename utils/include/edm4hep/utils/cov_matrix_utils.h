#ifndef EDM4HEP_UTILS_COVMATRIXUTILS_H
#define EDM4HEP_UTILS_COVMATRIXUTILS_H

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <type_traits>

namespace edm4hep {

namespace utils {

  namespace detail {
// From c++23 this is functionality offerd by the STL
#if __cpp_lib_to_underlying
    using to_index = std::to_underlying;
#else
    // Otherwise it is simple enough to roll our own
    template <typename E>
    constexpr auto to_index(E e) {
      return static_cast<std::underlying_type_t<E>>(e);
    }
#endif

    /// Cast an index to an enum value
    template <typename DimEnum>
    constexpr DimEnum to_enum(DimEnum index) {
      return static_cast<DimEnum>(index);
    }

    /// Need a constexpr swap for integers before c++20
    constexpr void swap(int& i, int& j) {
      int tmp = j;
      j = i;
      i = tmp;
    }
  } // namespace detail

  /**
   * Get the dimension of the covariance matrix from the size of the 1D array in
   * which it is stored
   *
   * **NOTE: to avoid having to do a square root operation and in order to keep
   * this constexpr this is currently only implemented for storage sizes up to
   * 21 (corresponding to covariance matrix dimensions of 6 x 6)**
   *
   * @param N the size of the 1D storage
   *
   * @returns the dimension of the covariance matrix
   */
  inline constexpr std::size_t get_cov_dim(std::size_t N) {
    switch (N) {
    case 21:
      return 6;
    case 15:
      return 5;
    case 10:
      return 4;
    case 6:
      return 3;
    case 3:
      return 2;
    case 1:
      return 1;
    }

    // We simply use throwing an exception to make compilation fail in constexpr
    // cases.
    throw std::invalid_argument("Not a valid size for a covariance matrix stored in lower triangular form");
  }

  /**
   * Transform a 2D index to a 1D index in lower triangular storage.
   *
   * @param i row index
   * @param j column index
   *
   * @returns the index into the 1D storage
   */
  constexpr int to_lower_tri(int i, int j) {
    if (i < j) {
      detail::swap(i, j);
    }
    return i * (i + 1) / 2 + j;
  }

  template <typename DimEnum, typename Scalar, std::size_t N>
  constexpr Scalar get_cov_value(const std::array<Scalar, N>& cov, DimEnum dimI, DimEnum dimJ) {
    const auto i = detail::to_index(dimI);
    const auto j = detail::to_index(dimJ);

    const auto dim = get_cov_dim(N);
    if (i < 0 || j < 0 || i >= dim || j >= dim) {
      // TODO: error handling
    }

    return cov[to_lower_tri(i, j)];
  }

  template <typename DimEnum, typename Scalar, std::size_t N>
  constexpr void set_cov_value(Scalar value, std::array<Scalar, N>& cov, DimEnum dimI, DimEnum dimJ) {
    auto i = detail::to_index(dimI);
    auto j = detail::to_index(dimJ);

    const auto dim = get_cov_dim(N);
    if (i < 0 || j < 0 || i >= dim || j >= dim) {
      // TODO: error handling
    }

    // Covariance is in lower triangle this brings us from 2D indices to 1D
    cov[to_lower_tri(i, j)] = value;
  }
} // namespace utils

} // namespace edm4hep

#endif // EDM4HEP_UTILS_COVMATRIXUTILS_H
