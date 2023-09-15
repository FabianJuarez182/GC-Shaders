#pragma once
#include <array>
#include <vector>
#include <glm/glm.hpp>

struct Face
{
  std::array<int, 3> vertexIndices;
  std::array<int, 3> normalIndices;
  std::array<int, 3> texIndices;
};


bool loadOBJ(
  const std::string& path,
  std::vector<glm::vec3> &out_vertices,
  std::vector<glm::vec3> &out_normals,
  std::vector<glm::vec3> &out_texcoords,
  std::vector<Face>& out_faces
);
