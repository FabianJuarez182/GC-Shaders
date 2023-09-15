#pragma once
#include "color.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>

// Declare the functions and global variables
extern SDL_Surface* currentTexture;
bool loadTexture(const std::string& filename);
Color getPixelFromTexture(float u, float v);
glm::vec3 getNormalFromTexture(float u, float v);