#include "FastNoiseLite.h"
#include "NoiseGenerator.hpp"

#include <cstdint>
#include <vector>

NoiseGenerator::NoiseGenerator(std::uint32_t seed, float frequency) {
  this->noise_.SetSeed(static_cast<int>(seed));
  this->noise_.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
  this->noise_.SetFrequency(frequency);
}

float NoiseGenerator::sample(size_t x, size_t y) const {
  return this->noise_.GetNoise(static_cast<float>(x), static_cast<float>(y));
}

std::vector<float> NoiseGenerator::generate_map(size_t width, size_t height) const {
  if (width <= 0 || height <= 0) { return {}; }

  std::vector<float> map(width * height);

  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      map[y * width + x] = this->sample(x, y);
    }
  }

  return map;
}