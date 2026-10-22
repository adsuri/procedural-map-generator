#include "TerrainGenerator.hpp"

int main(int argc, char **argv) {
  TerrainGenerator tg(7890, 2500, 2500);
  tg.generate_map();
  tg.output_map(std::cout);
}