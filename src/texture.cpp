#include "texture.h"

// Global SDL_Surface pointer
SDL_Surface* currentTexture = nullptr;

// Function to load texture from a file
bool loadTexture(const std::string& filename) {
    // Free the existing texture if there is one
    if (currentTexture != nullptr) {
        SDL_FreeSurface(currentTexture);
        currentTexture = nullptr;
    }

    // Load the image into an SDL_Surface
    //SDL_Surface* loadedSurface = IMG_Load(filename.c_str());
    //if (loadedSurface == nullptr) {
      //  std::cerr << "Unable to load image " << filename << "! SDL_image Error: " << IMG_GetError() << std::endl;
        //return false;
    //}

    // If the image has an alpha channel, we'll convert the loaded surface to a format that matches the display format
    // if (loadedSurface->format->BytesPerPixel == 4) {
      //  SDL_Surface* convertedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_ARGB8888, 0);
       // SDL_FreeSurface(loadedSurface);
        //if (convertedSurface == nullptr) {
         //   std::cerr << "Unable to convert image " << filename << "! SDL Error: " << SDL_GetError() << std::endl;
           // return false;
        //}
        //loadedSurface = convertedSurface;
    // }

    // currentTexture = loadedSurface;
    return true;
}

// Helper function to get pixel color from texture
Color getPixelFromTexture(float u, float v) {
    int texWidth = currentTexture->w;
    int texHeight = currentTexture->h;

    // Convert the normalized texture coordinates into the pixel space
    int x = static_cast<int>(u * texWidth);
    int y = static_cast<int>((1.0f - v) * texHeight);

    // Ensure x and y are within the texture's bounds
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= texWidth) x = texWidth - 1;
    if (y >= texHeight) y = texHeight - 1;

    // Fetch the pixel value from the texture
    Uint32 pixel = ((Uint32*)currentTexture->pixels)[y * currentTexture->pitch / sizeof(Uint32) + x];
    
    // Decompose the pixel into its RGBA components
    Uint8 r, g, b, a;
    SDL_GetRGBA(pixel, currentTexture->format, &r, &g, &b, &a);
    
    return Color(r, g, b, a);
}

// Helper function to get normal vector from a texture
glm::vec3 getNormalFromTexture(float u, float v) {
    int texWidth = currentTexture->w;
    int texHeight = currentTexture->h;

    // Convert the normalized texture coordinates into the pixel space
    int x = static_cast<int>(u * texWidth);
    int y = static_cast<int>((1.0f - v) * texHeight);

    // Ensure x and y are within the texture's bounds
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= texWidth) x = texWidth - 1;
    if (y >= texHeight) y = texHeight - 1;

    // Fetch the pixel value from the texture
    Uint32 pixel = ((Uint32*)currentTexture->pixels)[y * currentTexture->pitch / sizeof(Uint32) + x];
    
    // Decompose the pixel into its RGBA components
    Uint8 r, g, b, a;
    SDL_GetRGBA(pixel, currentTexture->format, &r, &g, &b, &a);

    // Convert the color data to a normal vector
    float xNormal = (r / 255.0f) * 2.0f - 1.0f;
    float yNormal = (g / 255.0f) * 2.0f - 1.0f;
    float zNormal = (b / 255.0f) * 2.0f - 1.0f;

    return glm::vec3(xNormal, yNormal, zNormal);
}
