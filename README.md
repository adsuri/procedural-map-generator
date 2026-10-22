# Procedural Map Generator

A C++17 program that generates procedural terrain maps using layered noise functions. It assigns terrain types based on elevation, temperature, and humidity, producing maps with several different types of biomes.

## Build and Run

From the project root:

- Run `make configure` once to configure CMake.
- Run `make release` to build the program in Release mode.
- Run `./build/mapgen > [output file]` to generate a map and save it as a PPM image.

This program is intended for use with linux/WSL.

## Options

The program supports command-line options to customize the map:

- `-s`, `--seed`: Set the random seed.
- `-w`, `--width`: Set the map width.
- `-h`, `--height`: Set the map height.

The noise parameters are tuned to look best at around a 500 x 500 size, so going much larger will make the map look too regular.

Example:

```bash
./build/mapgen --seed 42 --width 500 --height 500 > map.ppm
```

The output uses binary PPM (`P6`) format. You can open it with compatible image software or convert it into another image format.