#pragma once
#include "./FastNoise.h"
#include <vector>

constexpr int NOISE_WIDTH = 512;
constexpr int NOISE_HEIGHT = 512;

FastNoiseLite noise;

void setupNoise() {
  noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2); // Set the noise type to Perlin
}
