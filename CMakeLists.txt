cmake_minimum_required(VERSION 3.10)
project(MyOpenGLApp LANGUAGES CXX)

# 1. Set C++ Standard (C++17 is usually good for modern OpenGL apps)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 2. Find Dependencies
# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLFW (standard naming is glfw3)
find_package(glfw3 3.3 REQUIRED)

set(STB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/stb")
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/imgui")
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)


# 3. Define the Executable
# We point to main.cpp inside the src directory
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/Utils.cpp
    src/ParticleSystem.cpp
    src/Recorder.cpp
    src/RecordingTool.cpp
    src/Logging.cpp
    src/ParticleSystemApp.cpp
    ${IMGUI_SOURCES}
)

# 4. Include Directories
# This tells the compiler to look in your "include" folder for stb_image_write.h
target_include_directories(${PROJECT_NAME} PRIVATE 
    include
    ${STB_DIR}
    ${IMGUI_DIR} 
    ${IMGUI_DIR}/backends
)

# 5. Link Libraries
# This links the OpenGL and GLFW binaries to your app
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    OpenGL::GL 
    glfw
    ${CMAKE_DL_LIBS}
)
