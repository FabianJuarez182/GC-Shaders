#pragma once
#include <glm/glm.hpp>
#include "FastNoiseLite.h"
#include "uniforms.h"
#include "fragment.h"
#include "noise.h"
#include "print.h"

enum class FragmentShaderType {
    Stripes,
    Urano,
    Mars,
    Earth,
    Heat,
    Sun,
    Moon
};


Fragment fragmentShaderUrano(Fragment& fragment) {
    // Define el color base para Urano (un tono de azul verdoso)
    glm::vec3 uranoColor = glm::vec3(0.45f, 0.65f, 0.75f);

    // Aplica la intensidad de iluminación
    uranoColor *= fragment.intensity;

    // Establece el color del fragmento
    fragment.color = Color(uranoColor.r, uranoColor.g, uranoColor.b);

    return fragment;
}



Vertex vertexShader(const Vertex& vertex, const Uniforms& uniforms) {
    // Apply transformations to the input vertex using the matrices from the uniforms
    glm::vec4 clipSpaceVertex = uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex.position, 1.0f);

    // Perspective divide
    glm::vec3 ndcVertex = glm::vec3(clipSpaceVertex) / clipSpaceVertex.w;

    // Apply the viewport transform
    glm::vec4 screenVertex = uniforms.viewport * glm::vec4(ndcVertex, 1.0f);
    
    // Transform the normal
    glm::vec3 transformedNormal = glm::mat3(uniforms.model) * vertex.normal;
    transformedNormal = glm::normalize(transformedNormal);

    glm::vec3 transformedWorldPosition = glm::vec3(uniforms.model * glm::vec4(vertex.position, 1.0f));

    // Return the transformed vertex as a vec3
    return Vertex{
        glm::vec3(screenVertex),
        transformedNormal,
        vertex.tex,
        transformedWorldPosition,
        vertex.position
    };
}

Color interpolateColor(const Color& color1, const Color& color2, float t) {
    t = glm::clamp(t, 0.0f, 1.0f); // Asegúrate de que t esté en el rango [0, 1]
    return Color(
        static_cast<int>((1.0f - t) * color1.r + t * color2.r),
        static_cast<int>((1.0f - t) * color1.g + t * color2.g),
        static_cast<int>((1.0f - t) * color1.b + t * color2.b)
    );
}

Fragment fragmentShaderJupiter(Fragment& fragment) {

    // Define a color for Jupiter. You may want to adjust this to better match Jupiter's appearance.
    Color baseColor = Color(187, 151, 109);  // Color base de Júpiter (amarillo/marrón claro)
    Color lineColor = Color(87, 67, 45);     // Color para las líneas (marrón oscuro)
    Color redSpotColor = Color(255, 0, 0);   // Color de la Gran Mancha Roja (rojo)

    // Agrega la Gran Mancha Roja
    glm::vec2 redSpotCenter = glm::vec2(0.3f, 0.2f); // Posición relativa de la mancha roja
    float redSpotSize = 0.05f; // Tamaño de la mancha roja
    glm::vec2 fragmentPos2D = glm::vec2(fragment.originalPos.x, fragment.originalPos.y); // Convierte a vec2
    float distanceToRedSpot = glm::length(fragmentPos2D - redSpotCenter);

    float stripePattern = glm::abs(glm::cos(fragment.originalPos.y * 20.0f));

    // Interpolate between the base color and a darker version of the base color based on the stripe pattern.
    // This will create dark stripes on the sphere.
    Color stripeColor = baseColor * (0.8f + 0.2f * stripePattern);

    // Apply lighting intensity
    stripeColor = stripeColor * fragment.intensity;

    // Set the fragment color
    Color color = stripeColor;

    // Define the direction to the center of the circle in world space
    // Apply lighting intensity
    color = color * fragment.intensity;

    // Set the fragment color
    fragment.color = color;

    return fragment;
}

// Función para generar ruido utilizando FastNoise Lite
float generateNoise(float x, float y, float z) {
    FastNoiseLite noise;
    int offsetX = 1000;
    int offsetY = 1000;
    float offsetZ = 0.6f;
    int scale = 1000;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(0.005f);

    // Ajusta el umbral para menos continentes
    float continentThreshold = 0.4f;

    float normalizedValue = noise.GetNoise(((x * scale) + offsetX) * offsetZ, ((y * scale) + offsetY) * offsetZ, (z * scale)* offsetZ);
    
    // Compara el valor normalizado con el umbral para determinar continente u océano
    return (normalizedValue < continentThreshold) ? 1.0f : 0.0f;
}

// Función para generar nubes volumétricas
float generateCloudDensity(float x, float y, float z) {
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.07f);

    float scale = 100.0f; // Ajusta la escala para nubes más grandes
    float cloudDensity = noise.GetNoise(x * scale, y * scale, z * scale);

    // Ajusta la densidad de nubes para hacerlas más difusas
    cloudDensity = (cloudDensity + 1.0f) / 2.0f;

    // Define un umbral para la generación de nubes
    float cloudThreshold = 0.68f; // Umbral para generar nubes

    // Si la densidad de nubes supera el umbral, se generan nubes
    if (cloudDensity > cloudThreshold) {
        return cloudDensity;
    } else {
        return 0.0f; // Sin nubes en otras ubicaciones
    }
}

Fragment fragmentShaderMoon(Fragment& fragment) {
    // Crea un nuevo fragmento para modificar la posición si es necesario
    Fragment modifiedFragment = fragment;

    // Color base de la Luna
    glm::vec3 moonColor = glm::vec3(0.7f, 0.7f, 0.7f); // Color gris claro

    modifiedFragment.color = Color(moonColor.x, moonColor.y, moonColor.z);
    // Devuelve el fragmento modificado
    return modifiedFragment;
}

Fragment fragmentShaderEarth(Fragment& fragment) {
    Color NorthPole;
    Color SouthPole;
    Color continent;
    Color Ocean;
    Color Clouds; // Capa para las nubes
    glm::vec3 oceanColor = glm::vec3(0, 0, 128);  // Dark blue
    glm::vec3 continentColor = glm::vec3(34, 139, 34);  // Forest green
    glm::vec3 cloudColor = glm::vec3(255, 255, 255);  // Color de las nubes

    float continentPattern = glm::smoothstep(0.40f, 1.0f, fragment.originalPos.y);
    float continentPatternSouth = glm::smoothstep(-0.35f, -1.0f, fragment.originalPos.y);

    glm::vec3 c = glm::mix(oceanColor, continentColor, continentPattern);
    glm::vec3 d = glm::mix(oceanColor, continentColor, continentPatternSouth);

    // Define umbrales para separar el Polo Norte del Polo Sur
    float northPoleThreshold = 0.40f;  // Coordenada Y para el Polo Norte
    float southPoleThreshold = -0.35f;  // Coordenada Y para el Polo Sur

    // Comprueba si la coordenada Y está en el Polo Norte o en el Polo Sur
    if (fragment.originalPos.y > northPoleThreshold) {
        // Si está en el Polo Norte, aplica el color del Polo Norte
        Color color = Color(c.x, c.y, c.z);
        // Apply lighting intensity
        color = color * fragment.intensity;
        // Set the fragment color
        NorthPole = color;
    } else if (fragment.originalPos.y < southPoleThreshold) {
        // Si está en el Polo Sur, aplica el color del Polo Sur
        Color color = Color(d.x, d.y, d.z);
        // Aplica intensidad de iluminación
        color = color * fragment.intensity;
        // Establece el color del fragmento
        SouthPole = color;
    } else {
         // Genera nubes procedurales
        float cloudDensity = generateCloudDensity(fragment.originalPos.x, fragment.originalPos.y, fragment.originalPos.z);
        Color cloudLayer = Color(cloudColor.x / 255.0f, cloudColor.y / 255.0f, cloudColor.z / 255.0f);
        Clouds = cloudLayer * cloudDensity;
        // Si no está en los polos, considera esta región como continente u océano
        float noiseValue = generateNoise(fragment.originalPos.x, fragment.originalPos.y, fragment.originalPos.z);
        float continentThreshold = 0.1f;
        // Verifica si es un continente en base al ruido
        if (noiseValue < continentThreshold) {
            // Si es un continente, aplica el color del continente (Verde)
            continent = Color(continentColor.x / 255.0f, continentColor.y / 255.0f, continentColor.z / 255.0f); // Normaliza el color a valores en el rango [0, 1]
        } else {
            // Si no es continente, aplica el color del océano (Azul oscuro)
            Ocean = Color(oceanColor.x / 255.0f, oceanColor.y / 255.0f, oceanColor.z / 255.0f); // Normaliza el color a valores en el rango [0, 1]
        }
    }

    // Combina todos los componentes de color
    Color finalColor = NorthPole + SouthPole + Ocean + continent + Clouds;

    fragment.color = finalColor;

    return fragment;
}

Fragment fragmentShaderMars(Fragment& fragment) {
// Define el color base para Marte (rojo)
    glm::vec3 marsColor = glm::vec3(0.7f, 0.0f, 0.0f); // Rojo característico de Marte

    // Simula la topografía de Marte con montañas y valles (utilizando ruido)
    float elevation = generateNoise(fragment.originalPos.x, fragment.originalPos.y, fragment.originalPos.z);

    // Añade casquetes polares blancos
    float polarCapElevation = generateNoise(fragment.originalPos.x, fragment.originalPos.y, fragment.originalPos.z + 0.1f); // Cambia la capa polar ligeramente hacia arriba
    if (fragment.originalPos.y > 0.7f + polarCapElevation) {
        // Ajusta la mezcla para el casquete polar
        float polarMix = glm::smoothstep(0.7f, 0.8f, fragment.originalPos.y);
        marsColor = glm::mix(marsColor, glm::vec3(1.0f, 1.0f, 1.0f), polarMix);
    }

    // Simula la atmósfera de Marte y la dispersión de la luz (efecto de "cielo rojo")
    glm::vec3 sunlightColor = glm::vec3(1.0f, 1.0f, 1.0f); // Color de la luz solar

    // Ajusta la dispersión atmosférica en función de la altura
    float atmosphericScattering = glm::smoothstep(0.7f, 1.0f, fragment.originalPos.y);

    // Añade un efecto de "cielo rojo" basado en la dispersión
    marsColor = marsColor + (sunlightColor - marsColor) * atmosphericScattering;

    // Aplica detalles de textura realista utilizando ruido Perlin
    float textureDetail = generateNoise(fragment.originalPos.x * 10.0f, fragment.originalPos.y * 10.0f, fragment.originalPos.z * 10.0f);
    marsColor += glm::vec3(textureDetail * 0.1f);

    // Aplica la intensidad de iluminación
    marsColor *= fragment.intensity;

    // Establece el color del fragmento
    fragment.color = Color(marsColor.r, marsColor.g, marsColor.b);

    return fragment;
}

Fragment fragmentShaderHeat(Fragment& fragment) {
    // Define los colores para representar el calor 
    glm::vec3 hotColor = glm::vec3(1.0f, 0.0f, 0.0f);  // Rojo
    glm::vec3 warmColor = glm::vec3(1.0f, 1.0f, 0.0f);  // Amarillo

    // Calcula una variable de "calor" en función de la posición del fragment
    float heatValue = (glm::sin(fragment.originalPos.x * 10.0f) + glm::cos(fragment.originalPos.y * 10.0f)) * 0.5f + 0.5f;

    // Interpola entre los colores de "calor" (de frío a caliente) en función del valor de "calor"
    glm::vec3 interpolatedColor = glm::mix(warmColor, hotColor, heatValue);

    // Aplica la intensidad de iluminación (puedes modificar esto según tus necesidades)
    interpolatedColor = interpolatedColor * fragment.intensity;

    // Convierte el color a tu clase Color si es necesario
    fragment.color = Color(interpolatedColor.r, interpolatedColor.g, interpolatedColor.b);

    return fragment;
}


Fragment fragmentShaderSun(Fragment& fragment) {
// Define la posición del centro del sol (
    glm::vec3 sunCenter = glm::vec3(0.0f, 0.0f, 0.0f);

    // Calcula el vector desde la posición del fragmento hacia el centro del sol
    glm::vec3 fragmentToSun = sunCenter - fragment.worldPos;

    // Calcula la distancia desde el fragmento al centro del sol
    float distanceToSun = glm::length(fragmentToSun);

    // Define el radio del sol
    float sunRadius = 1.0f;

    // Calcula el factor de brillo basado en la distancia
    float glowFactor = glm::smoothstep(sunRadius, sunRadius * 1.2f, distanceToSun);

    // Define colores para cada capa
    glm::vec3 innerColor = glm::vec3(1.0f, 1.0f, 0.0f);  // Amarillo en el centro
    glm::vec3 middleColor = glm::vec3(1.0f, 0.5f, 0.0f);  // Naranja en el medio
    glm::vec3 outerColor = glm::vec3(1.0f, 0.0f, 0.0f);  // Rojo en la orilla

    // Combina colores de capas utilizando un suavizado
    glm::vec3 finalColor = innerColor * glowFactor + middleColor * (1.0f - glowFactor) + outerColor * (1.0f - glowFactor) * (1.0f - glowFactor);

    // Aplica la intensidad de iluminación (puedes ajustar esto según tus necesidades)
    finalColor =  finalColor * fragment.intensity;

    glm::vec3 spotColor = glm::vec3(1.0f, 1.0f, 0.0f);  // Amarillo para las manchas
    float randomValue = glm::fract(glm::sin(fragment.worldPos.x * 5.0f) * 43758.5453f);
    finalColor = glm::mix(finalColor, spotColor, randomValue * 0.4f);  // Ajusta el valor 0.5f para controlar la cantidad de manchas

    // Convierte el color a tu clase Color si es necesario
    fragment.color = Color(finalColor.r, finalColor.g, finalColor.b);

    return fragment;
}

Fragment fragmentShader(Fragment& fragment, FragmentShaderType shaderType) {
    switch (shaderType) {
        case FragmentShaderType::Stripes:
            return fragmentShaderJupiter(fragment);
        case FragmentShaderType::Urano:
            return fragmentShaderUrano(fragment);
        case FragmentShaderType::Mars:
            return fragmentShaderMars(fragment);
        case FragmentShaderType::Earth:
            return fragmentShaderEarth(fragment);
        case FragmentShaderType::Heat:
            return fragmentShaderHeat(fragment);
        case FragmentShaderType::Sun:
            return fragmentShaderSun(fragment);
        case FragmentShaderType::Moon:
            return fragmentShaderMoon(fragment);
        default:
            return fragment;
    }
}

