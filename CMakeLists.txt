cmake_minimum_required(VERSION 3.10)

project(GraphicsEngine)

set(CMAKE_CXX_STANDARD 17)

add_executable(GraphicsEngine
    main.cpp
    WindowHandler.cpp
    ConsoleMenu.cpp
    RenderPipeline.cpp
    RenderAction.cpp
)
target_link_libraries(GraphicsEngine gdi32 user32)