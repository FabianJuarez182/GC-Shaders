#pragma once
#include <glm/glm.hpp>
#include "color.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec3 tex;
  glm::vec3 worldPos;
  glm::vec3 originalPos;
};
