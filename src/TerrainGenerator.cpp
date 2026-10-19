#include "NoiseGenerator.hpp"
#include "TerrainGenerator.hpp"
#include "util.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>

TerrainGenerator::TerrainGenerator(std::uint32_t seed, size_t w, size_t h)
 : coarse_elevation_noise_(util::generate_seed(seed, 0), 0.0075f),
   medium_elevation_noise_(util::generate_seed(seed, 1), 0.03f),
   fine_elevation_noise_(util::generate_seed(seed, 2), 0.06f),
   temperature_noise_(util::generate_seed(seed, 3), 0.01f),
   humidity_noise_(util::generate_seed(seed, 4), 0.01f),
   width_(w),
   height_(h) {}

void TerrainGenerator::generate_map() {
  this->map_.reserve(this->width_ * this->height_);

  for (size_t y = 0; y < this->height_; ++y) {
    for (size_t x = 0; x < this->width_; ++x) {
      size_t idx = y * this->width_ + x;
      float elevation = this->coarse_elevation_noise_.sample(x, y);
      elevation += this->medium_elevation_noise_.sample(x, y) / 5.0f;
      elevation += this->fine_elevation_noise_.sample(x, y) / 10.0f;
      
      if (elevation >= 0.2f) {
        this->map_[idx] = TerrainType_::LAND;
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