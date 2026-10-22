#ifndef NOISE_GENERATOR_HPP
#define NOISE_GENERATOR_HPP

#include "FastNoiseLite.h"

#include <cstdint>
#include <vector>

class NoiseGenerator {
 private:
  FastNoiseLite noise_;

 public:
  explicit NoiseGenerator(std::uint32_t seed, float frequency, FastNoiseLite::NoiseType noise_type);

  float sample(size_t x, size_t y) const;

  std::vector<float> generate_map(size_t width, size_t height) const;
};

#endif