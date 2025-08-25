#ifndef MATH_DIMENSION_HPP
#define MATH_DIMENSION_HPP

#include <array>
#include <cmath>
#include <complex>

namespace MD {

template <typename T> struct MathematicalDimension {
  static constexpr int dim = 1;
  static constexpr std::array<T, 2> points = {-1, +1};
};

template <typename T> struct MathematicalDimension<std::complex<T>> {
  static constexpr int dim = 2;
  static constexpr std::array<std::complex<T>, 4> points = {
      std::complex<T>(-1, -1), std::complex<T>(-1, +1), std::complex<T>(+1, -1),
      std::complex<T>(+1, +1)};
};

template <typename T> constexpr int getMathematicalDimension() {
  return MathematicalDimension<T>::dim;
}

template <typename T> constexpr auto getMathematicalVertex() {
  return MathematicalDimension<T>::points;
}

template <typename T> constexpr auto getMathematicalNumberVertex() {
  return MathematicalDimension<T>::points.size();
}

} // namespace MD

#endif // MATH_DIMENSION_HPP