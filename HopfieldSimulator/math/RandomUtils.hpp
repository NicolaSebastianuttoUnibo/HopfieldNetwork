#ifndef RANDOM_UTILS_HPP
#define RANDOM_UTILS_HPP

#include <random>

namespace RU{

inline std::mt19937& getRandomGenerator(uint32_t seed_value = std::random_device{}()) {
    static std::mt19937 gen(seed_value); 
    return gen;
}

}

#endif