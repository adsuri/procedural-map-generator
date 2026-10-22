#ifndef TERRAIN_GENERATOR_HPP
#define TERRAIN_GENERATOR_HPP

#include "FastNoiseLite.h"
#include "NoiseGenerator.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

class TerrainGenerator {
 private:
  struct Color_ {
    std::uint8_t r;
    std::uint8_t g;
    std::uint8_t b;
  };

  enum class TerrainType_ : char {
    WATER,
    DEEP_WATER,
    LAND,
    MOUNTAIN
  };

  inline static const Color_ COLOR_MAP_[] = {
    {65, 130, 190}, // WATER
    {45, 100, 155}, // DEEP_WATER
    {100, 175, 85}, // LAND
    {135, 135, 135} // MOUNTAIN
  };

  NoiseGenerator coarse_elevation_noise_;
  NoiseGenerator medium_elevation_noise_;
  NoiseGenerator fine_elevation_noise_;

  NoiseGenerator mountain_region_noise_;
  NoiseGenerator mountain_ridge_noise_;

  NoiseGenerator mountain_warp_x_;
  NoiseGenerator mountain_warp_y_;

  NoiseGenerator temperature_noise_;
  NoiseGenerator humidity_noise_;

  size_t width_;
  size_t height_;
  std::vector<TerrainType_> map_;

  friend std::ostream &operator<<(std::ostream &os, TerrainType_ terrain);

 public:
  explicit TerrainGenerator(std::uint32_t seed, size_t w, size_t h);

  void generate_map();

  void output_map(std::ostream &os);
};

#endif