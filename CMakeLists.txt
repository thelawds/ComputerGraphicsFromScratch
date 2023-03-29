cmake_minimum_required(VERSION 3.21)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

add_executable(
        RayTracing
        main.cpp
        common/SdlWindow.h
        common/SdlWindow.cpp
        common/Canvas.h
        common/Canvas.cpp
        common/Vector3D.h world/Sphere.h world/Viewport.h common/Color.h world/Lights.h renderers/Renderer.h world/World.h renderers/RayTracingRenderer.cpp renderers/RayTracingRenderer.h world/Camera.h)

target_link_libraries(
        RayTracing
        PRIVATE
        SDL2
)
