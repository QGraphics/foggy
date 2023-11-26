cmake_minimum_required(VERSION 3.26)
project(foggy)

set(CMAKE_CXX_STANDARD 17)

add_executable(foggy src/main.cpp
        src/rendering/ShaderProgram.cpp
        src/rendering/ShaderProgram.h
        src/rendering/Texture.cpp
        src/rendering/Texture.h
        src/rendering/Mesh.h
        src/rendering/Camera.cpp
        src/rendering/Camera.h
        src/rendering/Mesh.cpp
)

include_directories("/usr/include/GLFW" "/usr/include/GL")

target_link_libraries(foggy -std=c++11 -Wall -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lSOIL -lXinerama -lXcursor)

