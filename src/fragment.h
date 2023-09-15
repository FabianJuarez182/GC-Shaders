#pragma once
#include <glm/glm.hpp>
#include <cstdint>
#include "color.h"

struct Fragment {
  uint16_t x;      
  uint16_t y;      
  double z;  // z-buffer
  Color color; // r, g, b values for color
  float intensity;  // light intensity
  glm::vec3 worldPos;
  glm::vec3 originalPos;
};

struct FragColor {
  Color color;
  double z; // instead of z-buffer
};

