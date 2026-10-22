#include "NoiseGenerator.hpp"
#include "TerrainGenerator.hpp"
#include "util.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iostream>

TerrainGenerator::TerrainGenerator(std::uint32_t seed, size_t w, size_t h)
 : coarse_elevation_noise_(util::generate_seed(seed, 0), 0.0075, FastNoiseLite::NoiseType::NoiseType_Perlin),
   medium_elevation_noise_(util::generate_seed(seed, 1), 0.03f, FastNoiseLite::NoiseType::NoiseType_Perlin),
   fine_elevation_noise_(util::generate_seed(seed, 2), 0.06f, FastNoiseLite::NoiseType::NoiseType_Perlin),
   mountain_region_noise_(util::generate_seed(seed, 3), 0.004f, FastNoiseLite::NoiseType::NoiseType_Perlin),
   mountain_ridge_noise_(util::generate_seed(seed, 4), 0.012f, FastNoiseLite::NoiseType::NoiseType_Perlin),
   mountain_warp_x_(util::generate_seed(seed, 5), 0.006f, FastNoiseLite::NoiseType::NoiseType_Perlin),
   mountain_warp_y_(util::generate_seed(seed, 6), 0.006f, FastNoiseLite::NoiseType::NoiseType_Perlin),
   coarse_temperature_noise_(util::generate_seed(seed, 7), 0.001f, FastNoiseLite::NoiseType::NoiseType_Perlin),
   medium_temperature_noise_(util::generate_seed(seed, 7), 0.025f, FastNoiseLite::NoiseType::NoiseType_Perlin),
   coarse_humidity_noise_(util::generate_seed(seed, 8), 0.001f, FastNoiseLite::NoiseType::NoiseType_Perlin),
   medium_humidity_noise_(util::generate_seed(seed, 7), 0.025f, FastNoiseLite::NoiseType::NoiseType_Perlin),
   width_(w),
   height_(h) {}

TerrainGenerator::TerrainType_ TerrainGenerator::choose_biome_(float temperature, float humidity) const {
  if (temperature < -0.35f) {
    if (humidity > 0.2f) {
      return TerrainType_::TAIGA;
    }
    return TerrainType_::TUNDRA;
  }

  if (temperature > 0.35f) {
    if (humidity < -0.2f) {
      return TerrainType_::DESERT;
    }
    if (humidity > 0.3f) {
      return TerrainType_::TROPICAL_FOREST;
    }
    return TerrainType_::SAVANNA;
  }

  if (humidity > 0.2f) {
    return TerrainType_::FOREST;
  }

  return TerrainType_::PLAINS;
}

void TerrainGenerator::generate_map() {
  this->map_.reserve(this->width_ * this->height_);

  for (size_t y = 0; y < this->height_; ++y) {
    for (size_t x = 0; x < this->width_; ++x) {
      size_t idx = y * this->width_ + x;
      float elevation = this->coarse_elevation_noise_.sample(x, y);
      elevation += this->medium_elevation_noise_.sample(x, y) / 5.0f;
      elevation += this->fine_elevation_noise_.sample(x, y) / 10.0f;

      float mountain_region = this->mountain_region_noise_.sample(x, y);

      float region_strength = std::clamp((mountain_region - 0.1f) / 0.6f, 0.0f, 1.0f);

      float warp_x = this->mountain_warp_x_.sample(x, y);
      float warp_y = this->mountain_warp_y_.sample(x, y);

      float ridge_x = static_cast<float>(x) + warp_x * 10.0f;
      float ridge_y = static_cast<float>(y) + warp_y * 10.0f;

      float ridge_noise = this->mountain_ridge_noise_.sample(ridge_x * 0.15f, ridge_y);

      float ridged = 1.0f - std::abs(ridge_noise);

      ridged = std::clamp((ridged - 0.55f) / 0.35f, 0.0f, 1.0f);

      bool is_mountain =
          elevation >= 0.55f
          && region_strength > 0.0f
          && ridged > 0.5f;
      
      if (is_mountain) {
        this->map_[idx] = TerrainType_::MOUNTAIN;

      } else if (elevation >= 0.2f) {
        float temperature = this->coarse_temperature_noise_.sample(x, y);
        temperature += this->medium_temperature_noise_.sample(x, y) / 5.0f;

        float humidity = this->coarse_humidity_noise_.sample(x, y);
        humidity += this->medium_humidity_noise_.sample(x, y) / 5.0f;

        this->map_[idx] = this->choose_biome_(temperature, humidity);

      } else if (elevation >= -0.10f) {
        this->map_[idx] = TerrainType_::WATER;
      } else {
        this->map_[idx] = TerrainType_::DEEP_WATER;
      }
    }
  }
}

void TerrainGenerator::output_map(std::ostream &os) {
  os << "P3\n";
  os << this->width_ << " " << this->height_ << "\n";
  os << "255\n";
  
  for (size_t y = 0; y < this->height_; ++y) {
    for (size_t x = 0; x < this->width_; ++x) {
      size_t idx = y * this->width_ + x;

      os << this->map_[idx];
    }
    os << "\n";
  }
}

std::ostream &operator<<(std::ostream &os, TerrainGenerator::TerrainType_ terrain) {
  size_t index = static_cast<int>(terrain);
  TerrainGenerator::Color_ color = TerrainGenerator::COLOR_MAP_[index];

  os << static_cast<int>(color.r) << " "
    << static_cast<int>(color.g) << " "
    << static_cast<int>(color.b) << " ";
  return os;
}