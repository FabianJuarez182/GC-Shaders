#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "fragment.h"

std::vector<Fragment> line(const glm::vec3& v1, const glm::vec3& v2);
