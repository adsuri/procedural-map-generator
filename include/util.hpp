#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdint>

namespace util {
  std::uint32_t generate_seed(std::uint32_t seed, std::uint32_t salt);
}

#endif