cmake_minimum_required(VERSION 3.22.1)
project(flappy_bird VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)
# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLFW
find_package(glfw3 REQUIRED)

# Add the executable
add_executable(flappy_bird src/main.cpp)

# Link GLFW and OpenGL libraries
target_link_libraries(flappy_bird glfw)

target_link_libraries(flappy_bird OpenGL::GL)
    