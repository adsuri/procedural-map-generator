#include "TerrainGenerator.hpp"

int main(int argc, char **argv) {
  TerrainGenerator tg(465, 1000, 1000);
  tg.generate_map();
  tg.output_map(std::cout);
}