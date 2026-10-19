#include "util.hpp"

std::uint32_t util::generate_seed(std::uint32_t seed, std::uint32_t salt) {
  std::uint32_t x = seed + salt + 0x9e3779b9u;
  x = (x ^ (x >> 16)) * 0x85ebca6bu;
  x = (x ^ (x >> 13)) * 0xc2b2ae35u;
  x ^= x >> 16;
  return x;
}