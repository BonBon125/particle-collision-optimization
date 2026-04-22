cmake_minimum_required(VERSION 3.10)
project(MyOpenGLApp LANGUAGES CXX)

# 1. Set C++ Standard (C++17 is usually good for modern OpenGL apps)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Find Dependencies
# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLFW (standard naming is glfw3)
find_package(glfw3 3.3 REQUIRED)

# 3. Define the Executable
# We point to main.cpp inside the src directory
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/Utils.cpp
    src/ParticleSystem.cpp
    src/Recorder.cpp
)

# 4. Include Directories
# This tells the compiler to look in your "include" folder for stb_image_write.h
target_include_directories(${PROJECT_NAME} PRIVATE include)

# 5. Link Libraries
# This links the OpenGL and GLFW binaries to your app
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    OpenGL::GL 
    glfw
)

# Optional: Useful for identifying where files are in IDEs like Visual Studio
source_group("Header Files" FILES include/stb_image_write.h)
source_group("Source Files" FILES src/main.cpp)
