#include "TerrainGenerator.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <getopt.h>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  int option_index = 0, option = 0;
  opterr = false;

  struct option long_opts[] = {
    { "seed", required_argument, nullptr, 's' },
    { "width", required_argument, nullptr, 'w' },
    { "height", required_argument, nullptr, 'h' },
    { nullptr, 0, nullptr, '\0' }
  };

  std::uint32_t seed = 8675309;
  size_t width = 2500;
  size_t height = 2500;

  while ((option = getopt_long(argc, argv, "s:w:h:", long_opts, &option_index)) != -1) {
    switch (option) {
      case 's':
        try {
          seed = static_cast<uint32_t>(std::stoul(std::string(optarg)));
        } catch (const std::invalid_argument& e) {
          std::cerr << "Invalid argument: Argument for 'seed' could not be converted." << std::endl;
        }
        catch (const std::out_of_range& e) {
          std::cerr << "Out of range: The value is too large for a uint32_t." << std::endl;
        }
        break;

      case 'w':
        try {
          width = static_cast<size_t>(std::stoull(std::string(optarg)));
        } catch (const std::invalid_argument& e) {
          std::cerr << "Invalid argument: Argument for 'width' could not be converted." << std::endl;
        }
        break;

      case 'h':
        try {
          height = static_cast<size_t>(std::stoull(std::string(optarg)));
        } catch (const std::invalid_argument& e) {
          std::cerr << "Invalid argument: Argument for 'height' could not be converted." << std::endl;
        }
        break;
    }
  }

  TerrainGenerator tg(seed, width, height);

  const auto start = std::chrono::steady_clock::now();
  tg.output_map(std::cout);
  const auto end = std::chrono::steady_clock::now();

  const auto elapsed = std::chrono::duration<double, std::milli>(end - start).count();

  std::cerr << "Generated map in " << elapsed << " ms\n";
}