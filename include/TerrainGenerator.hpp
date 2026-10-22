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
    MOUNTAIN,
    PLAINS,
    DESERT,
    FOREST,
    TROPICAL_FOREST,
    TAIGA,
    TUNDRA,
    SAVANNA
  };

  inline static const Color_ COLOR_MAP_[] = {
    {65, 130, 190}, // WATER
    {45, 100, 155}, // DEEP_WATER
    {135, 135, 135}, // MOUNTAIN
    {100, 175, 85}, // PLAINS
    {220, 195, 105}, // DESERT
    {45, 120, 55}, // FOREST
    {25, 100, 45}, // TROPICAL_FOREST
    {100, 125, 95}, // TAIGA
    {190, 195, 200}, // TUNDRA
    {190, 165, 80} // SAVANNA
  };

  NoiseGenerator coarse_elevation_noise_;
  NoiseGenerator medium_elevation_noise_;
  NoiseGenerator fine_elevation_noise_;

  NoiseGenerator mountain_region_noise_;
  NoiseGenerator mountain_ridge_noise_;

  NoiseGenerator mountain_warp_x_;
  NoiseGenerator mountain_warp_y_;

  NoiseGenerator coarse_temperature_noise_;
  NoiseGenerator medium_temperature_noise_;

  NoiseGenerator coarse_humidity_noise_;
  NoiseGenerator medium_humidity_noise_;

  size_t width_;
  size_t height_;
  std::vector<TerrainType_> map_;

  TerrainType_ choose_biome_(float temperature, float humidity) const;

  friend std::ostream &operator<<(std::ostream &os, TerrainType_ terrain);

 public:
  explicit TerrainGenerator(std::uint32_t seed, size_t w, size_t h);

  void generate_map();

  void output_map(std::ostream &os);
};

#endif