#ifndef MATH_DIMENSION_HPP
#define MATH_DIMENSION_HPP

#include <complex>
#include <array>
#include <cmath>

namespace MD {

template <typename T>
struct MathematicalDimension {
    static constexpr int dim = 1;
    static constexpr std::array<T, 2> points = {-1, +1};
};

template <typename T>
struct MathematicalDimension<std::complex<T>> {
    static constexpr int dim = 2;
    static constexpr std::array<std::complex<T>, 4> points = {
        std::complex<T>(-1, -1),
        std::complex<T>(-1, +1),
        std::complex<T>(+1, -1),
        std::complex<T>(+1, +1)
    };
};

// Helper function to get the mathematical dimension value
template <typename T>
constexpr int getMathematicalDimension() {
    return MathematicalDimension<T>::dim;
}

// Helper function to get the mathematical vertices
template <typename T>
constexpr auto getMathematicalVertex() {
    return MathematicalDimension<T>::points;
}

}

#endif // MATH_DIMENSION_HPP