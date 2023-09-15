cmake_minimum_required(VERSION 3.10)

# set the project name
project(SR)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Release)

# set the profiling flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")

# find all .cpp files in the src directory
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# add the executable
add_executable(SR ${SOURCE_FILES})

# find and include SDL2, SDL_image, and GLM
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# threading

find_package(TBB REQUIRED)
target_link_libraries(${PROJECT_NAME} TBB::tbb)

# link libraries
target_link_libraries(SR ${SDL2_LIBRARIES} SDL2_image ${GLM_LIBRARIES})