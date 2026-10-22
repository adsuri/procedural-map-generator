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
   medium_temperature_noise_(util::generate_seed(seed, 8), 0.025f, FastNoiseLite::NoiseType::NoiseType_Perlin),
   coarse_humidity_noise_(util::generate_seed(seed, 9), 0.001f, FastNoiseLite::NoiseType::NoiseType_Perlin),
   medium_humidity_noise_(util::generate_seed(seed, 10), 0.025f, FastNoiseLite::NoiseType::NoiseType_Perlin),
   generated_map_(false),
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

TerrainGenerator::TerrainType_ TerrainGenerator::sample_map(size_t x, size_t y) const {
  float elevation = coarse_elevation_noise_.sample(x, y);
  elevation += medium_elevation_noise_.sample(x, y) / 5.0f;
  elevation += fine_elevation_noise_.sample(x, y) / 10.0f;

  if (elevation >= 0.55f) {
    const float mountain_region = mountain_region_noise_.sample(x, y);

    if (mountain_region > 0.1f) {
      const float warp_x = mountain_warp_x_.sample(x, y);
      const float warp_y = mountain_warp_y_.sample(x, y);

      const float ridge_x = static_cast<float>(x) + warp_x * 10.0f;
      const float ridge_y = static_cast<float>(y) + warp_y * 10.0f;

      const float ridge_noise = mountain_ridge_noise_.sample(ridge_x * 0.15f, ridge_y);

      const float ridged = 1.0f - std::abs(ridge_noise);

      if (ridged > 0.725f) {
        return TerrainType_::MOUNTAIN;
      }
    }
  }

  if (elevation >= 0.2f) {
    float temperature = coarse_temperature_noise_.sample(x, y);
    temperature += medium_temperature_noise_.sample(x, y) / 5.0f;

    float humidity = coarse_humidity_noise_.sample(x, y);
    humidity += medium_humidity_noise_.sample(x, y) / 5.0f;

    return this->choose_biome_(temperature, humidity);
  }

  return elevation >= -0.10f
    ? TerrainType_::WATER
    : TerrainType_::DEEP_WATER;
}

void TerrainGenerator::generate_map() {
  this->generated_map_ = true;

  this->map_.resize(this->width_ * this->height_);

  for (size_t y = 0; y < this->height_; ++y) {
    for (size_t x = 0; x < this->width_; ++x) {
      size_t idx = y * this->width_ + x;
      this->map_[idx] = this->sample_map(x, y);
    }
  }
}

void TerrainGenerator::output_map(std::ostream &os) {
  os << "P6\n";
  os << this->width_ << " " << this->height_ << "\n";
  os << "255\n";

  if (!this->generated_map_) {
    std::vector<char> row(width_ * 3);

    for (std::size_t y = 0; y < this->height_; ++y) {
      for (std::size_t x = 0; x < this->width_; ++x) {
        const auto& color = COLOR_MAP_[static_cast<std::size_t>(this->sample_map(x, y))];

        row[x * 3 + 0] = static_cast<char>(color.r);
        row[x * 3 + 1] = static_cast<char>(color.g);
        row[x * 3 + 2] = static_cast<char>(color.b);
      }

      os.write(row.data(), static_cast<std::streamsize>(row.size()));
    }
  } else {
    std::vector<char> row(width_ * 3);

    for (std::size_t y = 0; y < this->height_; ++y) {
      for (std::size_t x = 0; x < this->width_; ++x) {
        size_t idx = y * this->width_ + x;
        const auto& color = COLOR_MAP_[static_cast<std::size_t>(this->map_[idx])];

        row[x * 3 + 0] = static_cast<char>(color.r);
        row[x * 3 + 1] = static_cast<char>(color.g);
        row[x * 3 + 2] = static_cast<char>(color.b);
      }

      os.write(row.data(), static_cast<std::streamsize>(row.size()));
    }
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