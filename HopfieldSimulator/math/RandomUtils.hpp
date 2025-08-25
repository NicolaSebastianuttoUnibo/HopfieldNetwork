#ifndef RANDOM_UTILS_HPP
#define RANDOM_UTILS_HPP

#include <random>

namespace RU {

inline std::mt19937 &getRandomGenerator() {
  static std::mt19937 gen(std::random_device{}());
  return gen;
}

inline void seedGenerator(uint32_t seed_value = std::random_device{}()) {
  getRandomGenerator().seed(seed_value);
}

} // namespace RU

#endif // RANDOM_UTILS_HPP