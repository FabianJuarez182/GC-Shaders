#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "line.h"
#include "framebuffer.h"
#include "color.h"
#include "texture.h"

extern glm::vec3 L;

// Función para calcular las coordenadas baricéntricas de un punto en un triángulo
std::pair<float, float> barycentricCoordinates(const glm::ivec2& P, const glm::vec3& A, const glm::vec3& B, const glm::vec3& C);

// Función para rasterizar un triángulo y obtener fragmentos
std::vector<Fragment> triangle(const Vertex& a, const Vertex& b, const Vertex& c);
